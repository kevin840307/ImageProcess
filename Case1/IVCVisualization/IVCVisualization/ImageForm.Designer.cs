namespace IVCVisualization
{
    partial class ImageForm
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
            this.pic_image = new System.Windows.Forms.PictureBox();
            this.panel1 = new System.Windows.Forms.Panel();
            this.lab_data = new System.Windows.Forms.Label();
            ((System.ComponentModel.ISupportInitialize)(this.pic_image)).BeginInit();
            this.panel1.SuspendLayout();
            this.SuspendLayout();
            // 
            // pic_image
            // 
            this.pic_image.Location = new System.Drawing.Point(3, 12);
            this.pic_image.Name = "pic_image";
            this.pic_image.Size = new System.Drawing.Size(890, 602);
            this.pic_image.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pic_image.TabIndex = 0;
            this.pic_image.TabStop = false;
            // 
            // panel1
            // 
            this.panel1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.panel1.AutoScroll = true;
            this.panel1.Controls.Add(this.pic_image);
            this.panel1.Location = new System.Drawing.Point(12, 44);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(890, 570);
            this.panel1.TabIndex = 1;
            // 
            // lab_data
            // 
            this.lab_data.AutoSize = true;
            this.lab_data.Location = new System.Drawing.Point(15, 13);
            this.lab_data.Name = "lab_data";
            this.lab_data.Size = new System.Drawing.Size(37, 15);
            this.lab_data.TabIndex = 2;
            this.lab_data.Text = "資訊";
            // 
            // ImageForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 15F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(914, 626);
            this.Controls.Add(this.lab_data);
            this.Controls.Add(this.panel1);
            this.Name = "ImageForm";
            this.Text = "ImageForm";
            ((System.ComponentModel.ISupportInitialize)(this.pic_image)).EndInit();
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.PictureBox pic_image;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label lab_data;
    }
}