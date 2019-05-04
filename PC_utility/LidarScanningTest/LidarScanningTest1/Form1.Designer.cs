namespace LidarScanningTest1
{
    partial class Form1
    {
        /// <summary>
        /// Обязательная переменная конструктора.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Освободить все используемые ресурсы.
        /// </summary>
        /// <param name="disposing">истинно, если управляемый ресурс должен быть удален; иначе ложно.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Код, автоматически созданный конструктором форм Windows

        /// <summary>
        /// Требуемый метод для поддержки конструктора — не изменяйте 
        /// содержимое этого метода с помощью редактора кода.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.cmbPortList = new System.Windows.Forms.ComboBox();
            this.btnOpenClose = new System.Windows.Forms.Button();
            this.serialPort1 = new System.IO.Ports.SerialPort(this.components);
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.statusStrip1 = new System.Windows.Forms.StatusStrip();
            this.lblPacketCnt = new System.Windows.Forms.ToolStripStatusLabel();
            this.lblScanPeriod = new System.Windows.Forms.Label();
            this.lblScanFreq = new System.Windows.Forms.Label();
            this.radarPlotComponent1 = new LidarScanningTest1.RadarPlotComponent();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.numCoeffA = new System.Windows.Forms.NumericUpDown();
            this.numCoeffB = new System.Windows.Forms.NumericUpDown();
            this.label3 = new System.Windows.Forms.Label();
            this.numAngCorrection = new System.Windows.Forms.NumericUpDown();
            this.btnSaveCoeff = new System.Windows.Forms.Button();
            this.groupBox2 = new System.Windows.Forms.GroupBox();
            this.lblRawValue = new System.Windows.Forms.Label();
            this.lblDistValue = new System.Windows.Forms.Label();
            this.label4 = new System.Windows.Forms.Label();
            this.numericUpDown1 = new System.Windows.Forms.NumericUpDown();
            this.lblAVRValue = new System.Windows.Forms.Label();
            this.lblMaxMIn = new System.Windows.Forms.Label();
            this.groupBox1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numCoeffA)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numCoeffB)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numAngCorrection)).BeginInit();
            this.groupBox2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).BeginInit();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Controls.Add(this.cmbPortList);
            this.groupBox1.Controls.Add(this.btnOpenClose);
            this.groupBox1.Location = new System.Drawing.Point(9, 10);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(310, 62);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Serial Port";
            // 
            // cmbPortList
            // 
            this.cmbPortList.FormattingEnabled = true;
            this.cmbPortList.Location = new System.Drawing.Point(14, 22);
            this.cmbPortList.Name = "cmbPortList";
            this.cmbPortList.Size = new System.Drawing.Size(176, 24);
            this.cmbPortList.TabIndex = 2;
            // 
            // btnOpenClose
            // 
            this.btnOpenClose.Location = new System.Drawing.Point(199, 17);
            this.btnOpenClose.Name = "btnOpenClose";
            this.btnOpenClose.Size = new System.Drawing.Size(95, 34);
            this.btnOpenClose.TabIndex = 1;
            this.btnOpenClose.Text = "Open";
            this.btnOpenClose.UseVisualStyleBackColor = true;
            this.btnOpenClose.Click += new System.EventHandler(this.btnOpenClose_Click);
            // 
            // serialPort1
            // 
            this.serialPort1.BaudRate = 115200;
            this.serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(this.serialPort1_DataReceived);
            // 
            // timer1
            // 
            this.timer1.Interval = 50;
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // statusStrip1
            // 
            this.statusStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.statusStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.lblPacketCnt});
            this.statusStrip1.Location = new System.Drawing.Point(0, 637);
            this.statusStrip1.Name = "statusStrip1";
            this.statusStrip1.Size = new System.Drawing.Size(1072, 25);
            this.statusStrip1.TabIndex = 1;
            this.statusStrip1.Text = "statusStrip1";
            // 
            // lblPacketCnt
            // 
            this.lblPacketCnt.Name = "lblPacketCnt";
            this.lblPacketCnt.Size = new System.Drawing.Size(113, 20);
            this.lblPacketCnt.Text = "Packets count: 0";
            // 
            // lblScanPeriod
            // 
            this.lblScanPeriod.AutoSize = true;
            this.lblScanPeriod.Location = new System.Drawing.Point(337, 23);
            this.lblScanPeriod.Name = "lblScanPeriod";
            this.lblScanPeriod.Size = new System.Drawing.Size(116, 17);
            this.lblScanPeriod.TabIndex = 2;
            this.lblScanPeriod.Text = "Scan Period: N/A";
            // 
            // lblScanFreq
            // 
            this.lblScanFreq.AutoSize = true;
            this.lblScanFreq.Location = new System.Drawing.Point(336, 44);
            this.lblScanFreq.Name = "lblScanFreq";
            this.lblScanFreq.Size = new System.Drawing.Size(104, 17);
            this.lblScanFreq.TabIndex = 3;
            this.lblScanFreq.Text = "Scan Freq: N/A";
            // 
            // radarPlotComponent1
            // 
            this.radarPlotComponent1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.radarPlotComponent1.BorderStyle = System.Windows.Forms.BorderStyle.FixedSingle;
            this.radarPlotComponent1.Location = new System.Drawing.Point(11, 78);
            this.radarPlotComponent1.Name = "radarPlotComponent1";
            this.radarPlotComponent1.Size = new System.Drawing.Size(893, 552);
            this.radarPlotComponent1.TabIndex = 4;
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(497, 21);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(58, 17);
            this.label1.TabIndex = 5;
            this.label1.Text = "Coeff A:";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(497, 50);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(58, 17);
            this.label2.TabIndex = 6;
            this.label2.Text = "Coeff B:";
            // 
            // numCoeffA
            // 
            this.numCoeffA.DecimalPlaces = 7;
            this.numCoeffA.Increment = new decimal(new int[] {
            1,
            0,
            0,
            458752});
            this.numCoeffA.Location = new System.Drawing.Point(561, 19);
            this.numCoeffA.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numCoeffA.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.numCoeffA.Name = "numCoeffA";
            this.numCoeffA.Size = new System.Drawing.Size(100, 22);
            this.numCoeffA.TabIndex = 7;
            // 
            // numCoeffB
            // 
            this.numCoeffB.DecimalPlaces = 3;
            this.numCoeffB.Increment = new decimal(new int[] {
            1,
            0,
            0,
            196608});
            this.numCoeffB.Location = new System.Drawing.Point(561, 47);
            this.numCoeffB.Maximum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numCoeffB.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            -2147483648});
            this.numCoeffB.Name = "numCoeffB";
            this.numCoeffB.Size = new System.Drawing.Size(100, 22);
            this.numCoeffB.TabIndex = 8;
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(671, 21);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(85, 17);
            this.label3.TabIndex = 9;
            this.label3.Text = "Angul. corr.:";
            // 
            // numAngCorrection
            // 
            this.numAngCorrection.DecimalPlaces = 1;
            this.numAngCorrection.Increment = new decimal(new int[] {
            1,
            0,
            0,
            65536});
            this.numAngCorrection.Location = new System.Drawing.Point(762, 19);
            this.numAngCorrection.Maximum = new decimal(new int[] {
            20,
            0,
            0,
            0});
            this.numAngCorrection.Minimum = new decimal(new int[] {
            20,
            0,
            0,
            -2147483648});
            this.numAngCorrection.Name = "numAngCorrection";
            this.numAngCorrection.Size = new System.Drawing.Size(68, 22);
            this.numAngCorrection.TabIndex = 10;
            // 
            // btnSaveCoeff
            // 
            this.btnSaveCoeff.Location = new System.Drawing.Point(836, 12);
            this.btnSaveCoeff.Name = "btnSaveCoeff";
            this.btnSaveCoeff.Size = new System.Drawing.Size(68, 53);
            this.btnSaveCoeff.TabIndex = 11;
            this.btnSaveCoeff.Text = "Save Coeff.";
            this.btnSaveCoeff.UseVisualStyleBackColor = true;
            this.btnSaveCoeff.Click += new System.EventHandler(this.btnSaveCoeff_Click);
            // 
            // groupBox2
            // 
            this.groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox2.Controls.Add(this.lblMaxMIn);
            this.groupBox2.Controls.Add(this.lblAVRValue);
            this.groupBox2.Controls.Add(this.numericUpDown1);
            this.groupBox2.Controls.Add(this.label4);
            this.groupBox2.Controls.Add(this.lblDistValue);
            this.groupBox2.Controls.Add(this.lblRawValue);
            this.groupBox2.Location = new System.Drawing.Point(912, 10);
            this.groupBox2.Name = "groupBox2";
            this.groupBox2.Size = new System.Drawing.Size(152, 261);
            this.groupBox2.TabIndex = 12;
            this.groupBox2.TabStop = false;
            this.groupBox2.Text = "Data Analyse";
            // 
            // lblRawValue
            // 
            this.lblRawValue.AutoSize = true;
            this.lblRawValue.Location = new System.Drawing.Point(10, 30);
            this.lblRawValue.Name = "lblRawValue";
            this.lblRawValue.Size = new System.Drawing.Size(106, 17);
            this.lblRawValue.TabIndex = 0;
            this.lblRawValue.Text = "Raw Value: N/A";
            // 
            // lblDistValue
            // 
            this.lblDistValue.AutoSize = true;
            this.lblDistValue.Location = new System.Drawing.Point(10, 59);
            this.lblDistValue.Name = "lblDistValue";
            this.lblDistValue.Size = new System.Drawing.Size(94, 17);
            this.lblDistValue.TabIndex = 1;
            this.lblDistValue.Text = "Distance: N/A";
            // 
            // label4
            // 
            this.label4.Location = new System.Drawing.Point(27, 100);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(120, 38);
            this.label4.TabIndex = 2;
            this.label4.Text = "Number of analysed points:";
            this.label4.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // numericUpDown1
            // 
            this.numericUpDown1.Increment = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.numericUpDown1.Location = new System.Drawing.Point(39, 147);
            this.numericUpDown1.Maximum = new decimal(new int[] {
            50,
            0,
            0,
            0});
            this.numericUpDown1.Minimum = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.numericUpDown1.Name = "numericUpDown1";
            this.numericUpDown1.Size = new System.Drawing.Size(91, 22);
            this.numericUpDown1.TabIndex = 3;
            this.numericUpDown1.Value = new decimal(new int[] {
            5,
            0,
            0,
            0});
            this.numericUpDown1.ValueChanged += new System.EventHandler(this.numericUpDown1_ValueChanged);
            // 
            // lblAVRValue
            // 
            this.lblAVRValue.AutoSize = true;
            this.lblAVRValue.Location = new System.Drawing.Point(10, 198);
            this.lblAVRValue.Name = "lblAVRValue";
            this.lblAVRValue.Size = new System.Drawing.Size(92, 17);
            this.lblAVRValue.TabIndex = 4;
            this.lblAVRValue.Text = "Average: N/A";
            // 
            // lblMaxMIn
            // 
            this.lblMaxMIn.AutoSize = true;
            this.lblMaxMIn.Location = new System.Drawing.Point(10, 228);
            this.lblMaxMIn.Name = "lblMaxMIn";
            this.lblMaxMIn.Size = new System.Drawing.Size(86, 17);
            this.lblMaxMIn.TabIndex = 5;
            this.lblMaxMIn.Text = "MaxMin: N/A";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1072, 662);
            this.Controls.Add(this.groupBox2);
            this.Controls.Add(this.btnSaveCoeff);
            this.Controls.Add(this.numAngCorrection);
            this.Controls.Add(this.label3);
            this.Controls.Add(this.numCoeffB);
            this.Controls.Add(this.numCoeffA);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.radarPlotComponent1);
            this.Controls.Add(this.lblScanFreq);
            this.Controls.Add(this.lblScanPeriod);
            this.Controls.Add(this.statusStrip1);
            this.Controls.Add(this.groupBox1);
            this.Name = "Form1";
            this.Text = "LidarScanningTest";
            this.groupBox1.ResumeLayout(false);
            this.statusStrip1.ResumeLayout(false);
            this.statusStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numCoeffA)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numCoeffB)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numAngCorrection)).EndInit();
            this.groupBox2.ResumeLayout(false);
            this.groupBox2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDown1)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox groupBox1;
        private System.Windows.Forms.Button btnOpenClose;
        private System.IO.Ports.SerialPort serialPort1;
        private System.Windows.Forms.ComboBox cmbPortList;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.StatusStrip statusStrip1;
        private System.Windows.Forms.ToolStripStatusLabel lblPacketCnt;
        private System.Windows.Forms.Label lblScanPeriod;
        private System.Windows.Forms.Label lblScanFreq;
        private RadarPlotComponent radarPlotComponent1;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.NumericUpDown numCoeffA;
        private System.Windows.Forms.NumericUpDown numCoeffB;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown numAngCorrection;
        private System.Windows.Forms.Button btnSaveCoeff;
        private System.Windows.Forms.GroupBox groupBox2;
        private System.Windows.Forms.Label lblRawValue;
        private System.Windows.Forms.Label lblDistValue;
        private System.Windows.Forms.NumericUpDown numericUpDown1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.Label lblAVRValue;
        private System.Windows.Forms.Label lblMaxMIn;
    }
}

