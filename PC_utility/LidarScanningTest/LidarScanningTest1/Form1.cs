using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Security;
using System.Security.Permissions;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace LidarScanningTest1
{
    public struct RadarPoint
    {
        public double x;
        public double y;
        public double dist;
        public bool corr;
        public bool black;
    }

    public partial class Form1 : Form
    {
        // Array that includes list of COM ports names that are registered in the system
        List<String> ports = new List<string>();

        UartPacketsParser DataParser = new UartPacketsParser();

        public SettingsHandling settings_holder;//Used for storing settings

        int rx_data_status = 0;

        /// <summary>
        /// Rotation period in ms
        /// </summary>
        int RotationPeriod = 0;

        int LidarStatusFlags = 0;

        struct ScanPoint
        {
            public int raw_value;

            public double dist_m;
        }

        ScanPoint[] ScanPoints = new ScanPoint[360];

        RadarPoint[] RadarPoints = new RadarPoint[400];

        // Distance calculation coefficients
        double curr_coeff_a = 0;
        double curr_coeff_b = 0;
        double curr_base_length = 5.8;
        double curr_angular_correction = 4;//degrees


        public Form1()
        {
            InitializeComponent();

            ports.AddRange(GetSerialPortNames());

            cmbPortList.Items.Clear();
            cmbPortList.Items.AddRange(ports.ToArray<String>());

            if (ports.Count == 0)
            {
                MessageBox.Show("No serial ports in system!", "Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
                cmbPortList.Items.Add("1");
            }
            else
                cmbPortList.SelectedItem = cmbPortList.Items[0];

            string settings_file_path = Application.StartupPath + @"\config.ini";
            settings_holder = new SettingsHandling(settings_file_path);

            string serial_name = settings_holder.GetSetting("SERIAL_SETTINGS", "serial");
            if (cmbPortList.Items.Count > 0)
            {
                cmbPortList.Text = serial_name;
            }

            //Load calibration coefficients
            string coef_a_str = settings_holder.GetSetting("LIDAR_SETTINGS", "coef_a");
            string coef_b_str = settings_holder.GetSetting("LIDAR_SETTINGS", "coef_b");
            string base_length_str = settings_holder.GetSetting("LIDAR_SETTINGS", "base_length");
            string ang_corr_str = settings_holder.GetSetting("LIDAR_SETTINGS", "angular_corr");

            double coef_a = Convert.ToDouble(coef_a_str, System.Globalization.CultureInfo.InvariantCulture);
            double coef_b = Convert.ToDouble(coef_b_str, System.Globalization.CultureInfo.InvariantCulture);
            curr_base_length = Convert.ToDouble(base_length_str, System.Globalization.CultureInfo.InvariantCulture);
            curr_angular_correction = Convert.ToDouble(ang_corr_str, System.Globalization.CultureInfo.InvariantCulture);

            numCoeffA.Value = (decimal)coef_a;
            numCoeffB.Value = (decimal)coef_b;
            numAngCorrection.Value = (decimal)curr_angular_correction;

            timer1.Enabled = true;
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            if (DataParser.data_pending == true)
            {
                curr_coeff_a = (double)numCoeffA.Value;
                curr_coeff_b = (double)numCoeffB.Value;
                curr_angular_correction = (double)numAngCorrection.Value;

                int[] rx_data = DataParser.get_received_data();
                ProcessRxData(rx_data);
                lblPacketCnt.Text = "Packets count: " + DataParser.total_packets;
            }
        }

        /// <summary>
        /// Process packet received from the Lidar
        /// </summary>
        /// <param name="data_array"></param>
        void ProcessRxData(int[] data_array)
        {
            int i;
            int r_pos;

            RotationPeriod = data_array[1];
            LidarStatusFlags = data_array[0];

            for (i = 2; i < 362; i++)
            {
                r_pos = i - 2;
                ScanPoints[r_pos].raw_value = data_array[i];
                ScanPoints[r_pos].dist_m = ConvertToLength(data_array[i]);
            }

            CalculateRadarData();
            radarPlotComponent1.DrawRadar(RadarPoints);

            double freq = Math.Round(1.0 / (double)(RotationPeriod / 1000.0), 1);//ms -> sec
            lblScanPeriod.Text = "Scan Perod: " + RotationPeriod.ToString() + " ms";
            lblScanFreq.Text = "Scan Freq: " + freq.ToString() + " Hz";
        }

        void CalculateRadarData()
        {
            int i;
            double x = 0;
            double y = 0;
            double angle_rad = 0;
            double ofset3_rad = 0;//baseline correction
            double dist;
            int offset1_deg;
            double offset2_deg;//additional correction

            double ang5 = 90;

            for (i = 0; i < 360; i++)
            {
                dist = ScanPoints[i].dist_m;
                offset1_deg = i;//angle
                if (offset1_deg > 359)
                    offset1_deg = offset1_deg - 360;

                offset2_deg = (double)i / 360.0 * curr_angular_correction;//degrees
                ofset3_rad = Math.Atan(0.03 / dist);//radians
                angle_rad = (double)(offset1_deg + offset2_deg  + ang5) / 180.0 * (Math.PI) + ofset3_rad;

                RadarPoints[offset1_deg].dist = dist;
                RadarPoints[offset1_deg].x = (Math.Cos(angle_rad) * dist);
                RadarPoints[offset1_deg].y = (Math.Sin(angle_rad) * dist);

                //testing
                if (ScanPoints[offset1_deg].raw_value > 16384)
                    RadarPoints[offset1_deg].black = true;
                else
                    RadarPoints[offset1_deg].black = false;
            }
        }

        /// <summary>
        /// Convert subpixel value to distance
        /// </summary>
        /// <param name="pixel">Subpixel value</param>
        /// <returns>Distance value in meters</returns>
        double ConvertToLength(int pixel)
        {
            double dist2 = 0;//value in cm

            pixel = pixel & 16383;

            if (pixel > 50)
            {
                dist2 = (-curr_base_length) / Math.Tan((double)(pixel) * curr_coeff_a - curr_coeff_b); //<<<<<<<<<<<<<<<<
            }
            else
                dist2 = 0;

            return dist2 / 100.0;//convert to meters
        }

        private void btnOpenClose_Click(object sender, EventArgs e)
        {
            if (serialPort1.IsOpen == false)
            {
                //port is closed, try to open
                bool opened_ok = OpenPort();
                if (opened_ok)
                {
                    btnOpenClose.Text = "Close";
                    SaveSettings();
                }
            }
            else
            {
                //port is opened, try to close
                bool closed_ok = ClosePort();
                if (closed_ok)
                {
                    btnOpenClose.Text = "Open";
                }
            }
        }

        void SaveSettings()
        {
            string serial_name = serialPort1.PortName;
            settings_holder.AddSetting("SERIAL_SETTINGS", "serial", serial_name);
            settings_holder.SaveSettings();
        }

        //Return true if port had been opened
        bool OpenPort()
        {
            try
            {
                serialPort1.PortName = cmbPortList.Text;
                serialPort1.Open();
                System.Threading.Thread.Sleep(200);
                return serialPort1.IsOpen;
            }
            catch (Exception)
            {
                return false;
            }
        }

        //Return true if port had been closed
        bool ClosePort()
        {
            try
            {
                serialPort1.Close();
                return true;
            }
            catch (Exception)
            {
                return false;
            }
        }

        private void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            byte[] rx_data = new byte[128];
            int received_size = 0;

            try
            {
                received_size = serialPort1.BytesToRead;

                while (received_size > 127)
                {
                    serialPort1.Read(rx_data, 0, 127);
                    DataParser.process_received_data(rx_data, 127);
                    received_size = serialPort1.BytesToRead;
                }
                serialPort1.Read(rx_data, 0, received_size);
                DataParser.process_received_data(rx_data, received_size);
            }
            catch (Exception ee)
            {
                System.Diagnostics.Debug.WriteLine(ee.ToString());
            }
            rx_data_status = 1;//пришли новые данные
        }


        /// <summary>
        /// Return array with serial ports names that are in the system
        /// </summary>
        /// <returns></returns>
        public static string[] GetSerialPortNames()
        {
            RegistryKey registryKey = null;
            RegistryKey registryKey2 = null;
            string[] array = null;
            RegistryPermission registryPermission = new RegistryPermission(RegistryPermissionAccess.Read, "HKEY_LOCAL_MACHINE\\HARDWARE\\DEVICEMAP\\SERIALCOMM");
            registryPermission.Assert();
            try
            {
                registryKey = Registry.LocalMachine;
                registryKey2 = registryKey.OpenSubKey("HARDWARE\\DEVICEMAP\\SERIALCOMM", false);
                if (registryKey2 != null)
                {
                    string[] valueNames = registryKey2.GetValueNames();
                    array = new string[valueNames.Length];
                    for (int i = 0; i < valueNames.Length; i++)
                    {
                        array[i] = (string)registryKey2.GetValue(valueNames[i]);
                    }
                }
            }
            finally
            {
                if (registryKey != null)
                {
                    registryKey.Close();
                }
                if (registryKey2 != null)
                {
                    registryKey2.Close();
                }
                CodeAccessPermission.RevertAssert();
            }
            if (array == null)
            {
                array = new string[0];
            }
            return array;
        }

        private void btnSaveCoeff_Click(object sender, EventArgs e)
        {
            string coef_a_str = curr_coeff_a.ToString(System.Globalization.CultureInfo.InvariantCulture);
            string coef_b_str = curr_coeff_b.ToString(System.Globalization.CultureInfo.InvariantCulture);
            string ang_corr_str = curr_angular_correction.ToString(System.Globalization.CultureInfo.InvariantCulture);

            settings_holder.AddSetting("LIDAR_SETTINGS", "coef_a", coef_a_str);
            settings_holder.AddSetting("LIDAR_SETTINGS", "coef_b", coef_b_str);
            settings_holder.AddSetting("LIDAR_SETTINGS", "angular_corr", ang_corr_str);

            settings_holder.SaveSettings();
        }
    }
}
