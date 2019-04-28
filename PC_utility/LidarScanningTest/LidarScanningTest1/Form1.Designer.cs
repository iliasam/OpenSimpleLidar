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
            this.groupBox1.SuspendLayout();
            this.statusStrip1.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
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
            this.statusStrip1.Size = new System.Drawing.Size(694, 25);
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
            this.radarPlotComponent1.Size = new System.Drawing.Size(658, 516);
            this.radarPlotComponent1.TabIndex = 4;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(694, 662);
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
    }
}

