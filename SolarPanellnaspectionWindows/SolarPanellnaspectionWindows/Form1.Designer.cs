namespace SolarPanellnaspectionWindows
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.btn_detection_all = new System.Windows.Forms.Button();
            this.btn_screenshot_all = new System.Windows.Forms.Button();
            this.btn_detection = new System.Windows.Forms.Button();
            this.btn_stain = new System.Windows.Forms.Button();
            this.btn_point = new System.Windows.Forms.Button();
            this.btn_line = new System.Windows.Forms.Button();
            this.btn_open = new System.Windows.Forms.Button();
            this.btn_screenshot = new System.Windows.Forms.Button();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.btn_kmean_predict = new System.Windows.Forms.Button();
            this.btn_kmean_train = new System.Windows.Forms.Button();
            this.btn_kmean_setting = new System.Windows.Forms.Button();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.SuspendLayout();
            // 
            // pictureBox1
            // 
            this.pictureBox1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBox1.Location = new System.Drawing.Point(12, 99);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(897, 440);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage2);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(901, 81);
            this.tabControl1.TabIndex = 1;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.btn_detection_all);
            this.tabPage1.Controls.Add(this.btn_screenshot_all);
            this.tabPage1.Controls.Add(this.btn_detection);
            this.tabPage1.Controls.Add(this.btn_stain);
            this.tabPage1.Controls.Add(this.btn_point);
            this.tabPage1.Controls.Add(this.btn_line);
            this.tabPage1.Controls.Add(this.btn_open);
            this.tabPage1.Controls.Add(this.btn_screenshot);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(893, 52);
            this.tabPage1.TabIndex = 2;
            this.tabPage1.Text = "檢測";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // btn_detection_all
            // 
            this.btn_detection_all.Location = new System.Drawing.Point(748, 6);
            this.btn_detection_all.Name = "btn_detection_all";
            this.btn_detection_all.Size = new System.Drawing.Size(100, 40);
            this.btn_detection_all.TabIndex = 7;
            this.btn_detection_all.Text = "檢測資料夾";
            this.btn_detection_all.UseVisualStyleBackColor = true;
            this.btn_detection_all.Click += new System.EventHandler(this.btn_detection_all_Click);
            // 
            // btn_screenshot_all
            // 
            this.btn_screenshot_all.Location = new System.Drawing.Point(642, 6);
            this.btn_screenshot_all.Name = "btn_screenshot_all";
            this.btn_screenshot_all.Size = new System.Drawing.Size(100, 40);
            this.btn_screenshot_all.TabIndex = 6;
            this.btn_screenshot_all.Text = "剪裁資料夾";
            this.btn_screenshot_all.UseVisualStyleBackColor = true;
            this.btn_screenshot_all.Click += new System.EventHandler(this.btn_screenshot_all_Click);
            // 
            // btn_detection
            // 
            this.btn_detection.Location = new System.Drawing.Point(536, 6);
            this.btn_detection.Name = "btn_detection";
            this.btn_detection.Size = new System.Drawing.Size(100, 40);
            this.btn_detection.TabIndex = 5;
            this.btn_detection.Text = "所有定位";
            this.btn_detection.UseVisualStyleBackColor = true;
            this.btn_detection.Click += new System.EventHandler(this.btn_detection_Click);
            // 
            // btn_stain
            // 
            this.btn_stain.Location = new System.Drawing.Point(430, 6);
            this.btn_stain.Name = "btn_stain";
            this.btn_stain.Size = new System.Drawing.Size(100, 40);
            this.btn_stain.TabIndex = 4;
            this.btn_stain.Text = "定位汙點";
            this.btn_stain.UseVisualStyleBackColor = true;
            this.btn_stain.Click += new System.EventHandler(this.btn_stain_Click);
            // 
            // btn_point
            // 
            this.btn_point.Location = new System.Drawing.Point(324, 6);
            this.btn_point.Name = "btn_point";
            this.btn_point.Size = new System.Drawing.Size(100, 40);
            this.btn_point.TabIndex = 3;
            this.btn_point.Text = "定位點";
            this.btn_point.UseVisualStyleBackColor = true;
            this.btn_point.Click += new System.EventHandler(this.btn_point_Click);
            // 
            // btn_line
            // 
            this.btn_line.Location = new System.Drawing.Point(218, 6);
            this.btn_line.Name = "btn_line";
            this.btn_line.Size = new System.Drawing.Size(100, 40);
            this.btn_line.TabIndex = 2;
            this.btn_line.Text = "定位線";
            this.btn_line.UseVisualStyleBackColor = true;
            this.btn_line.Click += new System.EventHandler(this.btn_line_Click);
            // 
            // btn_open
            // 
            this.btn_open.Location = new System.Drawing.Point(6, 6);
            this.btn_open.Name = "btn_open";
            this.btn_open.Size = new System.Drawing.Size(100, 40);
            this.btn_open.TabIndex = 1;
            this.btn_open.Text = "開啟檔案";
            this.btn_open.UseVisualStyleBackColor = true;
            this.btn_open.Click += new System.EventHandler(this.btn_open_Click);
            // 
            // btn_screenshot
            // 
            this.btn_screenshot.Location = new System.Drawing.Point(112, 6);
            this.btn_screenshot.Name = "btn_screenshot";
            this.btn_screenshot.Size = new System.Drawing.Size(100, 40);
            this.btn_screenshot.TabIndex = 0;
            this.btn_screenshot.Text = "裁剪";
            this.btn_screenshot.UseVisualStyleBackColor = true;
            this.btn_screenshot.Click += new System.EventHandler(this.btn_screenshot_Click);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.btn_kmean_setting);
            this.tabPage2.Controls.Add(this.btn_kmean_predict);
            this.tabPage2.Controls.Add(this.btn_kmean_train);
            this.tabPage2.Location = new System.Drawing.Point(4, 25);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Size = new System.Drawing.Size(893, 52);
            this.tabPage2.TabIndex = 3;
            this.tabPage2.Text = "KMean";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // btn_kmean_predict
            // 
            this.btn_kmean_predict.Location = new System.Drawing.Point(112, 6);
            this.btn_kmean_predict.Name = "btn_kmean_predict";
            this.btn_kmean_predict.Size = new System.Drawing.Size(100, 40);
            this.btn_kmean_predict.TabIndex = 3;
            this.btn_kmean_predict.Text = "預測";
            this.btn_kmean_predict.UseVisualStyleBackColor = true;
            this.btn_kmean_predict.Click += new System.EventHandler(this.btn_kmean_predict_Click);
            // 
            // btn_kmean_train
            // 
            this.btn_kmean_train.Location = new System.Drawing.Point(6, 6);
            this.btn_kmean_train.Name = "btn_kmean_train";
            this.btn_kmean_train.Size = new System.Drawing.Size(100, 40);
            this.btn_kmean_train.TabIndex = 2;
            this.btn_kmean_train.Text = "訓練";
            this.btn_kmean_train.UseVisualStyleBackColor = true;
            this.btn_kmean_train.Click += new System.EventHandler(this.btn_kmean_train_Click);
            // 
            // btn_kmean_setting
            // 
            this.btn_kmean_setting.Location = new System.Drawing.Point(790, 6);
            this.btn_kmean_setting.Name = "btn_kmean_setting";
            this.btn_kmean_setting.Size = new System.Drawing.Size(100, 40);
            this.btn_kmean_setting.TabIndex = 4;
            this.btn_kmean_setting.Text = "設定";
            this.btn_kmean_setting.UseVisualStyleBackColor = true;
            this.btn_kmean_setting.Click += new System.EventHandler(this.btn_kmean_setting_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(925, 551);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.pictureBox1);
            this.Name = "Form1";
            this.Text = "Form1";
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button btn_screenshot;
        private System.Windows.Forms.Button btn_open;
        private System.Windows.Forms.Button btn_line;
        private System.Windows.Forms.Button btn_point;
        private System.Windows.Forms.Button btn_stain;
        private System.Windows.Forms.Button btn_detection;
        private System.Windows.Forms.Button btn_detection_all;
        private System.Windows.Forms.Button btn_screenshot_all;
        private System.Windows.Forms.TabPage tabPage2;
        private System.Windows.Forms.Button btn_kmean_train;
        private System.Windows.Forms.Button btn_kmean_predict;
        private System.Windows.Forms.Button btn_kmean_setting;
    }
}

