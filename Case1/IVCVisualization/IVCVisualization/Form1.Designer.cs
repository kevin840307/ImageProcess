namespace IVCVisualization
{
    partial class Form1
    {
        /// <summary>
        /// 設計工具所需的變數。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清除任何使用中的資源。
        /// </summary>
        /// <param name="disposing">如果應該處置 Managed 資源則為 true，否則為 false。</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 設計工具產生的程式碼

        /// <summary>
        /// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
        /// 這個方法的內容。
        /// </summary>
        private void InitializeComponent()
        {
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.btn_open = new System.Windows.Forms.Button();
            this.tabPage8 = new System.Windows.Forms.TabPage();
            this.ch_other_form = new System.Windows.Forms.CheckBox();
            this.checkBox4 = new System.Windows.Forms.CheckBox();
            this.checkBox3 = new System.Windows.Forms.CheckBox();
            this.ch_timely = new System.Windows.Forms.CheckBox();
            this.checkBox2 = new System.Windows.Forms.CheckBox();
            this.btn_per_tran = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.btn_bicubic = new System.Windows.Forms.Button();
            this.text_room = new System.Windows.Forms.TextBox();
            this.btn_linear = new System.Windows.Forms.Button();
            this.btn_nearest = new System.Windows.Forms.Button();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.btn_test = new System.Windows.Forms.Button();
            this.pic_src = new System.Windows.Forms.PictureBox();
            this.pic_grid = new System.Windows.Forms.PictureBox();
            this.pic_result = new System.Windows.Forms.PictureBox();
            this.pic_process = new System.Windows.Forms.PictureBox();
            this.pic_process2 = new System.Windows.Forms.PictureBox();
            this.tabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            this.tabPage8.SuspendLayout();
            this.tabPage4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pic_src)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_grid)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_result)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_process)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_process2)).BeginInit();
            this.SuspendLayout();
            // 
            // tabControl1
            // 
            this.tabControl1.Controls.Add(this.tabPage1);
            this.tabControl1.Controls.Add(this.tabPage8);
            this.tabControl1.Controls.Add(this.tabPage4);
            this.tabControl1.Location = new System.Drawing.Point(12, 12);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(987, 100);
            this.tabControl1.TabIndex = 7;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.btn_open);
            this.tabPage1.Location = new System.Drawing.Point(4, 25);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Size = new System.Drawing.Size(979, 71);
            this.tabPage1.TabIndex = 6;
            this.tabPage1.Text = "檔案";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // btn_open
            // 
            this.btn_open.Location = new System.Drawing.Point(4, 13);
            this.btn_open.Name = "btn_open";
            this.btn_open.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btn_open.Size = new System.Drawing.Size(100, 44);
            this.btn_open.TabIndex = 15;
            this.btn_open.Text = "開啟檔案";
            this.btn_open.UseVisualStyleBackColor = true;
            this.btn_open.Click += new System.EventHandler(this.btn_open_Click);
            // 
            // tabPage8
            // 
            this.tabPage8.Controls.Add(this.ch_other_form);
            this.tabPage8.Controls.Add(this.checkBox4);
            this.tabPage8.Controls.Add(this.checkBox3);
            this.tabPage8.Controls.Add(this.ch_timely);
            this.tabPage8.Controls.Add(this.checkBox2);
            this.tabPage8.Controls.Add(this.btn_per_tran);
            this.tabPage8.Controls.Add(this.label1);
            this.tabPage8.Controls.Add(this.btn_bicubic);
            this.tabPage8.Controls.Add(this.text_room);
            this.tabPage8.Controls.Add(this.btn_linear);
            this.tabPage8.Controls.Add(this.btn_nearest);
            this.tabPage8.Location = new System.Drawing.Point(4, 25);
            this.tabPage8.Name = "tabPage8";
            this.tabPage8.Size = new System.Drawing.Size(979, 71);
            this.tabPage8.TabIndex = 5;
            this.tabPage8.Text = "幾何變化";
            this.tabPage8.UseVisualStyleBackColor = true;
            // 
            // ch_other_form
            // 
            this.ch_other_form.AutoSize = true;
            this.ch_other_form.Location = new System.Drawing.Point(791, 37);
            this.ch_other_form.Name = "ch_other_form";
            this.ch_other_form.Size = new System.Drawing.Size(74, 19);
            this.ch_other_form.TabIndex = 25;
            this.ch_other_form.Text = "新視窗";
            this.ch_other_form.UseVisualStyleBackColor = true;
            // 
            // checkBox4
            // 
            this.checkBox4.AutoSize = true;
            this.checkBox4.Location = new System.Drawing.Point(661, 26);
            this.checkBox4.Name = "checkBox4";
            this.checkBox4.Size = new System.Drawing.Size(59, 19);
            this.checkBox4.TabIndex = 24;
            this.checkBox4.Text = "雙線";
            this.checkBox4.UseVisualStyleBackColor = true;
            this.checkBox4.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // checkBox3
            // 
            this.checkBox3.AutoSize = true;
            this.checkBox3.Location = new System.Drawing.Point(726, 26);
            this.checkBox3.Name = "checkBox3";
            this.checkBox3.Size = new System.Drawing.Size(59, 19);
            this.checkBox3.TabIndex = 23;
            this.checkBox3.Text = "三次";
            this.checkBox3.UseVisualStyleBackColor = true;
            this.checkBox3.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // ch_timely
            // 
            this.ch_timely.AutoSize = true;
            this.ch_timely.Location = new System.Drawing.Point(791, 12);
            this.ch_timely.Name = "ch_timely";
            this.ch_timely.Size = new System.Drawing.Size(59, 19);
            this.ch_timely.TabIndex = 21;
            this.ch_timely.Text = "即時";
            this.ch_timely.UseVisualStyleBackColor = true;
            // 
            // checkBox2
            // 
            this.checkBox2.AutoSize = true;
            this.checkBox2.Checked = true;
            this.checkBox2.CheckState = System.Windows.Forms.CheckState.Checked;
            this.checkBox2.Location = new System.Drawing.Point(596, 26);
            this.checkBox2.Name = "checkBox2";
            this.checkBox2.Size = new System.Drawing.Size(59, 19);
            this.checkBox2.TabIndex = 22;
            this.checkBox2.Text = "鄰近";
            this.checkBox2.UseVisualStyleBackColor = true;
            this.checkBox2.CheckedChanged += new System.EventHandler(this.checkBox2_CheckedChanged);
            // 
            // btn_per_tran
            // 
            this.btn_per_tran.Location = new System.Drawing.Point(490, 12);
            this.btn_per_tran.Name = "btn_per_tran";
            this.btn_per_tran.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btn_per_tran.Size = new System.Drawing.Size(100, 44);
            this.btn_per_tran.TabIndex = 20;
            this.btn_per_tran.Text = "透視變換";
            this.btn_per_tran.UseVisualStyleBackColor = true;
            this.btn_per_tran.Click += new System.EventHandler(this.btn_per_tran_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(322, 27);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(52, 15);
            this.label1.TabIndex = 10;
            this.label1.Text = "倍數：";
            // 
            // btn_bicubic
            // 
            this.btn_bicubic.Location = new System.Drawing.Point(216, 12);
            this.btn_bicubic.Name = "btn_bicubic";
            this.btn_bicubic.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btn_bicubic.Size = new System.Drawing.Size(100, 44);
            this.btn_bicubic.TabIndex = 19;
            this.btn_bicubic.Text = "三次插值";
            this.btn_bicubic.UseVisualStyleBackColor = true;
            this.btn_bicubic.Click += new System.EventHandler(this.btn_bicubic_Click);
            // 
            // text_room
            // 
            this.text_room.Location = new System.Drawing.Point(378, 24);
            this.text_room.Name = "text_room";
            this.text_room.Size = new System.Drawing.Size(100, 25);
            this.text_room.TabIndex = 9;
            this.text_room.Text = "1";
            this.text_room.KeyPress += new System.Windows.Forms.KeyPressEventHandler(this.text_room_KeyPress);
            // 
            // btn_linear
            // 
            this.btn_linear.Location = new System.Drawing.Point(110, 12);
            this.btn_linear.Name = "btn_linear";
            this.btn_linear.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btn_linear.Size = new System.Drawing.Size(100, 44);
            this.btn_linear.TabIndex = 15;
            this.btn_linear.Text = "雙線插值";
            this.btn_linear.UseVisualStyleBackColor = true;
            this.btn_linear.Click += new System.EventHandler(this.btn_linear_Click);
            // 
            // btn_nearest
            // 
            this.btn_nearest.Location = new System.Drawing.Point(4, 12);
            this.btn_nearest.Name = "btn_nearest";
            this.btn_nearest.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btn_nearest.Size = new System.Drawing.Size(100, 44);
            this.btn_nearest.TabIndex = 14;
            this.btn_nearest.Text = "鄰近內插";
            this.btn_nearest.UseVisualStyleBackColor = true;
            this.btn_nearest.Click += new System.EventHandler(this.btn_nearest_Click);
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.btn_test);
            this.tabPage4.Location = new System.Drawing.Point(4, 25);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Size = new System.Drawing.Size(979, 71);
            this.tabPage4.TabIndex = 1;
            this.tabPage4.Text = "測試";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // btn_test
            // 
            this.btn_test.Location = new System.Drawing.Point(4, 3);
            this.btn_test.Name = "btn_test";
            this.btn_test.RightToLeft = System.Windows.Forms.RightToLeft.No;
            this.btn_test.Size = new System.Drawing.Size(100, 44);
            this.btn_test.TabIndex = 4;
            this.btn_test.Text = "測試";
            this.btn_test.UseVisualStyleBackColor = true;
            this.btn_test.Click += new System.EventHandler(this.btn_test_Click);
            // 
            // pic_src
            // 
            this.pic_src.Location = new System.Drawing.Point(20, 126);
            this.pic_src.Name = "pic_src";
            this.pic_src.Size = new System.Drawing.Size(250, 250);
            this.pic_src.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pic_src.TabIndex = 8;
            this.pic_src.TabStop = false;
            // 
            // pic_grid
            // 
            this.pic_grid.Location = new System.Drawing.Point(538, 118);
            this.pic_grid.Name = "pic_grid";
            this.pic_grid.Size = new System.Drawing.Size(450, 559);
            this.pic_grid.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pic_grid.TabIndex = 9;
            this.pic_grid.TabStop = false;
            this.pic_grid.Paint += new System.Windows.Forms.PaintEventHandler(this.pic_grid_Paint);
            // 
            // pic_result
            // 
            this.pic_result.Location = new System.Drawing.Point(276, 126);
            this.pic_result.Name = "pic_result";
            this.pic_result.Size = new System.Drawing.Size(250, 250);
            this.pic_result.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pic_result.TabIndex = 9;
            this.pic_result.TabStop = false;
            // 
            // pic_process
            // 
            this.pic_process.Location = new System.Drawing.Point(20, 427);
            this.pic_process.Name = "pic_process";
            this.pic_process.Size = new System.Drawing.Size(250, 250);
            this.pic_process.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pic_process.TabIndex = 22;
            this.pic_process.TabStop = false;
            // 
            // pic_process2
            // 
            this.pic_process2.Location = new System.Drawing.Point(276, 427);
            this.pic_process2.Name = "pic_process2";
            this.pic_process2.Size = new System.Drawing.Size(250, 250);
            this.pic_process2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pic_process2.TabIndex = 23;
            this.pic_process2.TabStop = false;
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1014, 759);
            this.Controls.Add(this.pic_process2);
            this.Controls.Add(this.pic_process);
            this.Controls.Add(this.pic_src);
            this.Controls.Add(this.pic_result);
            this.Controls.Add(this.pic_grid);
            this.Controls.Add(this.tabControl1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.tabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage8.ResumeLayout(false);
            this.tabPage8.PerformLayout();
            this.tabPage4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pic_src)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_grid)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_result)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_process)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pic_process2)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.TabControl tabControl1;
        private System.Windows.Forms.TabPage tabPage8;
        private System.Windows.Forms.Button btn_linear;
        private System.Windows.Forms.Button btn_nearest;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.Button btn_test;
        private System.Windows.Forms.PictureBox pic_src;
        private System.Windows.Forms.TextBox text_room;
        private System.Windows.Forms.Button btn_bicubic;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox pic_grid;
        private System.Windows.Forms.Button btn_per_tran;
        private System.Windows.Forms.PictureBox pic_result;
        private System.Windows.Forms.CheckBox checkBox4;
        private System.Windows.Forms.CheckBox checkBox3;
        private System.Windows.Forms.CheckBox checkBox2;
        private System.Windows.Forms.CheckBox ch_timely;
        private System.Windows.Forms.PictureBox pic_process;
        private System.Windows.Forms.PictureBox pic_process2;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.Button btn_open;
        private System.Windows.Forms.CheckBox ch_other_form;
    }
}

