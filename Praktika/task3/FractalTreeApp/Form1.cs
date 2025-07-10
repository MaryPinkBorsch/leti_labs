using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;
using System.Linq;

namespace FractalTreeApp
{

    public partial class Form1 : Form
    {

        public class FractalNode
        {
            public int Level;

            public float TreeX, TreeY;

            public PointF[] WhiteBoxPoints = new PointF[4];
            public PointF[] BlackBoxPoints = new PointF[4];

            public PointF Center;
            public float Size;

            public List<FractalNode> Children = new List<FractalNode>();
            public bool IsVisible = true;
            public RectangleF TreeBounds;
            public FractalNode Parent = null;
        }

        private FractalNode root;
        private int maxDepth = 8;
        private int currentLevel = 0;
        private bool showAllLevels = false;

        private Panel fractalPanel;
        private Panel treePanel;
        private Panel referencePanel;
        private ComboBox levelSelector;
        private Button buildButton;
        private CheckBox showAllCheckBox;
        private TrackBar scaleTrackBar;
        private Label scaleLabel;
        private Label levelInfoLabel;
        private NumericUpDown maxDepthSelector;
        private Label maxDepthLabel;
        private double baseScale = 100000.0;

        private Point treeScrollOffset = new Point(0, 0);
        private bool isDragging = false;
        private Point lastMousePos;

        private double fractalScale = 1.0;
        private PointF fractalOffset = new PointF(0, 0);
        private bool isDraggingFractal = false;
        private Point lastFractalMousePos;

        private PictureBox referencePictureBox;

        private FractalNode hoveredNode = null;

        private double treeScale = 1.0;

        public Form1()
        {
            this.SetStyle(ControlStyles.AllPaintingInWmPaint |
                         ControlStyles.UserPaint |
                         ControlStyles.DoubleBuffer |
                         ControlStyles.ResizeRedraw, true);

            InitializeComponent();
            InitUI();
            BuildInitialFractal();
        }

        private void InitUI()
        {
            this.Text = "Фрактал: диагональные квадраты";
            this.Size = new Size(1150, 850);
            this.StartPosition = FormStartPosition.CenterScreen;

            fractalPanel = new Panel
            {
                Location = new Point(10, 10),
                Size = new Size(700, 600),
                BorderStyle = BorderStyle.FixedSingle,
                BackColor = Color.LightGray
            };

            typeof(Panel).InvokeMember("DoubleBuffered",
                System.Reflection.BindingFlags.SetProperty | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic,
                null, fractalPanel, new object[] { true });

            treePanel = new Panel
            {
                Location = new Point(720, 10),
                Size = new Size(400, 450),
                BorderStyle = BorderStyle.FixedSingle,
                BackColor = Color.LightGray,
                AutoScroll = false
            };

            typeof(Panel).InvokeMember("DoubleBuffered",
                System.Reflection.BindingFlags.SetProperty | System.Reflection.BindingFlags.Instance | System.Reflection.BindingFlags.NonPublic,
                null, treePanel, new object[] { true });

            //referencePanel = new Panel
            //{
            //    Location = new Point(1130, 10),
            //    Size = new Size(350, 300),
            //    BorderStyle = BorderStyle.FixedSingle,
            //    BackColor = Color.LightGray
            //};

            //referencePictureBox = new PictureBox
            //{
            //    ImageLocation = "D:\\zip vadim\\FractalTreeApp\\FractalTreeApp\\fractal.jpg"
            //};
            //referencePanel.Controls.Add(referencePictureBox);
            //referencePictureBox.Refresh();

            levelSelector = new ComboBox
            {
                Location = new Point(720, 470),
                Width = 120,
                DropDownStyle = ComboBoxStyle.DropDownList
            };

            buildButton = new Button
            {
                Location = new Point(850, 470),
                Text = "Построить",
                Size = new Size(80, 23)
            };

            showAllCheckBox = new CheckBox
            {
                Location = new Point(720, 500),
                Text = "Показать все уровни",
                Size = new Size(150, 20)
            };

            maxDepthLabel = new Label
            {
                Location = new Point(720, 580),
                Text = "Макс. уровень:",
                Size = new Size(100, 20)
            };

            maxDepthSelector = new NumericUpDown
            {
                Location = new Point(820, 578),
                Size = new Size(60, 23),
                Minimum = 1,
                Maximum = 15,
                Value = maxDepth
            };

            levelInfoLabel = new Label
            {
                Location = new Point(720, 610),
                Size = new Size(400, 100),
                Text = "Выберите уровень для отображения",
                BorderStyle = BorderStyle.FixedSingle,
                BackColor = Color.LightYellow
            };

            UpdateLevelSelector();

            buildButton.Click += BuildButton_Click;
            levelSelector.SelectedIndexChanged += LevelSelector_SelectedIndexChanged;
            showAllCheckBox.CheckedChanged += ShowAllCheckBox_CheckedChanged;
            maxDepthSelector.ValueChanged += MaxDepthSelector_ValueChanged;

            treePanel.MouseDown += TreePanel_MouseDown;
            treePanel.MouseMove += TreePanel_MouseMove;
            treePanel.MouseUp += TreePanel_MouseUp;
            treePanel.MouseClick += TreePanel_MouseClick;

            fractalPanel.MouseWheel += FractalPanel_MouseWheel;
            fractalPanel.MouseDown += FractalPanel_MouseDown;
            fractalPanel.MouseMove += FractalPanel_MouseMove;
            fractalPanel.MouseUp += FractalPanel_MouseUp;

            treePanel.MouseWheel += TreePanel_MouseWheel;

            this.Controls.AddRange(new Control[] {
                fractalPanel, treePanel, referencePanel, levelSelector, buildButton,
                showAllCheckBox, maxDepthLabel,
                maxDepthSelector, levelInfoLabel
            });

            fractalPanel.Paint += FractalPanel_Paint;
            treePanel.Paint += TreePanel_Paint;

            fractalPanel.TabStop = true;
            treePanel.TabStop = true;

            var fractalLabel = new Label
            {
                Location = new Point(10, 620),
                Text = "Фрактал (колесико мыши - бесконечное масштабирование, ЛКМ - перемещение)",
                Font = new Font("Arial", 10, FontStyle.Bold),
                Size = new Size(500, 40)
            };

            var treeLabel = new Label
            {
                Location = new Point(720, 720),
                Text = "Дерево построения (клик по узлу - показать уровень, колесико - масштаб)",
                Font = new Font("Arial", 10, FontStyle.Bold),
                Size = new Size(400, 40)
            };

            //var referenceLabel = new Label
            //{
            //    Location = new Point(1130, 320),
            //    Text = "Эталонное изображение",
            //    Font = new Font("Arial", 10),
            //    Size = new Size(300, 40)
            //};

            this.Controls.AddRange(new Control[] { fractalLabel, treeLabel });
        }

        private void UpdateLevelSelector()
        {
            levelSelector.Items.Clear();
            for (int i = 0; i <= maxDepth; i++)
                levelSelector.Items.Add($"Уровень {i}");

            if (levelSelector.Items.Count > 0)
                levelSelector.SelectedIndex = Math.Min(currentLevel, levelSelector.Items.Count - 1);
        }

        private void BuildInitialFractal()
        {

            root = new FractalNode
            {
                Level = 0,
                Center = new PointF(fractalPanel.Width / 2, fractalPanel.Height / 2),
                Size = (float)(150 * baseScale)
            };

            CalculateFractalCoordinates(root);

            GenerateFractalTree(root, maxDepth);

            CalculateTreeLayout(root, treePanel.Width / 2, 30, Math.Max(50, 200 - maxDepth * 10));

            UpdateDisplay();
        }

        private bool PointInRectangle(PointF RectangleCenter, float rectangleSide, PointF toCheck) 
        {
            if ((toCheck.X >= RectangleCenter.X - rectangleSide/2 && toCheck.X <= RectangleCenter.X + rectangleSide / 2) &&
             (toCheck.Y >= RectangleCenter.Y - rectangleSide / 2 && toCheck.Y <= RectangleCenter.Y + rectangleSide / 2))
                return true;
            return false;
        }

        private void GenerateFractalTree(FractalNode node, int maxLevel)
        {
            if (node.Level >= maxLevel)
                return;

            float newSize = node.Size / 2.0f;
            {
                PointF newCenter = new PointF(node.Center.X - node.Size/2.0f, node.Center.Y - node.Size / 2.0f);
                // добавляем ребенка только если его центр не лежит в деде
                if ((node.Parent == null) || !PointInRectangle(node.Parent.Center, node.Parent.Size, newCenter)) 
                {
                    var child = new FractalNode
                    {
                        Level = node.Level + 1,
                        Center = newCenter,
                        Size = newSize,
                        Parent = node
                    };

                    CalculateFractalCoordinates(child);

                    node.Children.Add(child);
                    GenerateFractalTree(child, maxLevel);
                }
            }
            {
                PointF newCenter = new PointF(node.Center.X + node.Size / 2.0f, node.Center.Y - node.Size / 2.0f);
                // добавляем ребенка только если его центр не лежит в деде
                if ((node.Parent == null) || !PointInRectangle(node.Parent.Center, node.Parent.Size, newCenter))
                {
                    var child = new FractalNode
                    {
                        Level = node.Level + 1,
                        Center = newCenter,
                        Size = newSize,
                        Parent = node
                    };

                    CalculateFractalCoordinates(child);

                    node.Children.Add(child);
                    GenerateFractalTree(child, maxLevel);
                }
            }
            {
                PointF newCenter = new PointF(node.Center.X - node.Size / 2.0f, node.Center.Y + node.Size / 2.0f);
                // добавляем ребенка только если его центр не лежит в деде
                if ((node.Parent == null) || !PointInRectangle(node.Parent.Center, node.Parent.Size, newCenter))
                {
                    var child = new FractalNode
                    {
                        Level = node.Level + 1,
                        Center = newCenter,
                        Size = newSize,
                        Parent = node
                    };

                    CalculateFractalCoordinates(child);

                    node.Children.Add(child);
                    GenerateFractalTree(child, maxLevel);
                }
            }
            {
                PointF newCenter = new PointF(node.Center.X + node.Size / 2.0f, node.Center.Y + node.Size / 2.0f);
                // добавляем ребенка только если его центр не лежит в деде
                if ((node.Parent == null) || !PointInRectangle(node.Parent.Center, node.Parent.Size, newCenter))
                {
                    var child = new FractalNode
                    {
                        Level = node.Level + 1,
                        Center = newCenter,
                        Size = newSize,
                        Parent = node
                    };

                    CalculateFractalCoordinates(child);

                    node.Children.Add(child);
                    GenerateFractalTree(child, maxLevel);
                }
            }
        }

        private void CalculateFractalCoordinates(FractalNode node)
        {
            // white box
            //левая верх точка квадрата
            node.WhiteBoxPoints[0] = new PointF(
                node.Center.X - node.Size/2.0f,
                node.Center.Y - node.Size/2.0f
            );
            //правая верх точка квадрата
            node.WhiteBoxPoints[1] = new PointF(
                node.Center.X + node.Size / 2.0f,
                node.Center.Y - node.Size / 2.0f
            );
            //правая низ точка квадрата
            node.WhiteBoxPoints[2] = new PointF(
                node.Center.X + node.Size / 2.0f,
                node.Center.Y + node.Size / 2.0f
            );
            //левая низ точка квадрата
            node.WhiteBoxPoints[3] = new PointF(
                node.Center.X - node.Size / 2.0f,
                node.Center.Y + node.Size / 2.0f
            );


            // black box
            // верх точка квадрата
            node.BlackBoxPoints[0] = new PointF(
                node.Center.X,
                node.Center.Y - node.Size * ((float) Math.Sqrt(2.0f) / 4.0f)
            );
            //правая  точка квадрата
            node.BlackBoxPoints[1] = new PointF(
                node.Center.X + node.Size * ((float)Math.Sqrt(2.0f) / 4.0f),
                node.Center.Y
            );
            // низ точка квадрата
            node.BlackBoxPoints[2] = new PointF(
                node.Center.X  ,
                node.Center.Y + node.Size * ((float)Math.Sqrt(2.0f) / 4.0f)
            );
            //левая  точка квадрата
            node.BlackBoxPoints[3] = new PointF(
                node.Center.X - node.Size * ((float)Math.Sqrt(2.0f) / 4.0f),
                node.Center.Y
            );
        }

        private void MaxDepthSelector_ValueChanged(object sender, EventArgs e)
        {
            maxDepth = (int)maxDepthSelector.Value;
            UpdateLevelSelector();
            BuildInitialFractal();
        }

        private void BuildButton_Click(object sender, EventArgs e)
        {
            BuildInitialFractal();
        }

        private void LevelSelector_SelectedIndexChanged(object sender, EventArgs e)
        {
            currentLevel = levelSelector.SelectedIndex;
            UpdateDisplay();
        }

        private void ShowAllCheckBox_CheckedChanged(object sender, EventArgs e)
        {
            showAllLevels = showAllCheckBox.Checked;
            UpdateDisplay();
        }

        private void FractalPanel_MouseWheel(object sender, MouseEventArgs e)
        {
            PointF mousePos = new PointF(e.X, e.Y);

            double scaleFactor = e.Delta > 0 ? 1.1 : 1.0 / 1.1;

            PointF worldMousePos = new PointF(
                (mousePos.X - fractalOffset.X) / (float)fractalScale,
                (mousePos.Y - fractalOffset.Y) / (float)fractalScale
            );

            fractalScale *= scaleFactor;
            fractalScale = Math.Max(0.000001, fractalScale);

            fractalOffset.X = mousePos.X - worldMousePos.X * (float)fractalScale;
            fractalOffset.Y = mousePos.Y - worldMousePos.Y * (float)fractalScale;

            UpdateScaleInfo();
            fractalPanel.Invalidate();
        }

        private void TreePanel_MouseWheel(object sender, MouseEventArgs e)
        {
            PointF mousePos = new PointF(e.X, e.Y);
            double scaleFactor = e.Delta > 0 ? 1.1 : 1.0 / 1.1;

            PointF worldMousePos = new PointF(
                (mousePos.X - treeScrollOffset.X) / (float)treeScale,
                (mousePos.Y - treeScrollOffset.Y) / (float)treeScale
            );

            treeScale *= scaleFactor;
            treeScale = Math.Max(0.01, treeScale);

            treeScrollOffset.X = (int)(mousePos.X - worldMousePos.X * treeScale);
            treeScrollOffset.Y = (int)(mousePos.Y - worldMousePos.Y * treeScale);

            treePanel.Invalidate();
        }

        private void UpdateScaleInfo()
        {
            double totalScale = baseScale * fractalScale;
            string scaleText;

            if (totalScale >= 1)
            {
                scaleText = $"Масштаб: {totalScale:F2}x";
            }
            else if (totalScale >= 0.01)
            {
                scaleText = $"Масштаб: 1/{1 / totalScale:F0}";
            }
            else
            {
                scaleText = $"Масштаб: 1/{1 / totalScale:E1}";
            }

            this.Text = $"Фрактал: диагональные квадраты - {scaleText}";
        }

        private void FractalPanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                isDraggingFractal = true;
                lastFractalMousePos = e.Location;
                fractalPanel.Cursor = Cursors.Hand;
            }
        }

        private void FractalPanel_MouseMove(object sender, MouseEventArgs e)
        {
            if (isDraggingFractal)
            {
                int deltaX = e.X - lastFractalMousePos.X;
                int deltaY = e.Y - lastFractalMousePos.Y;

                fractalOffset.X += deltaX;
                fractalOffset.Y += deltaY;

                lastFractalMousePos = e.Location;
                fractalPanel.Invalidate();
            }
        }

        private void FractalPanel_MouseUp(object sender, MouseEventArgs e)
        {
            isDraggingFractal = false;
            fractalPanel.Cursor = Cursors.Default;
        }

        private void TreePanel_MouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                isDragging = true;
                lastMousePos = e.Location;
                treePanel.Cursor = Cursors.Hand;
            }
        }

        private void TreePanel_MouseMove(object sender, MouseEventArgs e)
        {
            FractalNode nodeUnderMouse = GetNodeAtPoint(e.Location);
            if (nodeUnderMouse != hoveredNode)
            {
                hoveredNode = nodeUnderMouse;
                treePanel.Cursor = hoveredNode != null ? Cursors.Hand : Cursors.Default;
                treePanel.Invalidate();
            }

            if (isDragging)
            {
                int deltaX = e.X - lastMousePos.X;
                int deltaY = e.Y - lastMousePos.Y;

                treeScrollOffset.X += deltaX;
                treeScrollOffset.Y += deltaY;

                lastMousePos = e.Location;
                treePanel.Invalidate();
            }
        }

        private void TreePanel_MouseUp(object sender, MouseEventArgs e)
        {
            isDragging = false;
            treePanel.Cursor = Cursors.Default;
        }

        private void TreePanel_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Left)
            {
                FractalNode clickedNode = GetNodeAtPoint(e.Location);
                if (clickedNode != null)
                {
                    currentLevel = clickedNode.Level;
                    levelSelector.SelectedIndex = currentLevel;
                    UpdateDisplay();
                }
            }
        }

        private FractalNode GetNodeAtPoint(Point point)
        {
            return FindNodeAtPoint(root, point);
        }

        private FractalNode FindNodeAtPoint(FractalNode node, Point point)
        {
            float adjustedX = (point.X - treeScrollOffset.X) / (float)treeScale;
            float adjustedY = (point.Y - treeScrollOffset.Y) / (float)treeScale;

            float distance = (float)Math.Sqrt(
                Math.Pow(adjustedX - node.TreeX, 2) +
                Math.Pow(adjustedY - node.TreeY, 2)
            );

            float clickRadius = 15 / (float)treeScale;
            if (distance <= clickRadius)
            {
                return node;
            }

            foreach (var child in node.Children)
            {
                var result = FindNodeAtPoint(child, point);
                if (result != null)
                    return result;
            }

            return null;
        }

        private void UpdateDisplay()
        {
            int elementsCount = showAllLevels ? CountNodesUpToLevel(root, currentLevel) : CountNodesAtLevel(root, currentLevel);

            if (showAllLevels)
            {
                levelInfoLabel.Text = $"Отображаются все уровни от 0 до {currentLevel}\n" +
                                     $"Общее количество элементов: {elementsCount}\n";
            }
            else
            {
                levelInfoLabel.Text = $"Отображается уровень {currentLevel}\n" +
                                     $"Количество элементов: {elementsCount}\n";
            }

            UpdateScaleInfo();
            fractalPanel.Invalidate();
            treePanel.Invalidate();
        }

        private int CountNodesAtLevel(FractalNode node, int targetLevel)
        {
            if (node.Level == targetLevel)
                return 1;

            int count = 0;
            foreach (var child in node.Children)
                count += CountNodesAtLevel(child, targetLevel);

            return count;
        }

        private int CountNodesUpToLevel(FractalNode node, int maxLevel)
        {
            int count = 0;
            if (node.Level <= maxLevel)
                count = 1;

            foreach (var child in node.Children)
                count += CountNodesUpToLevel(child, maxLevel);

            return count;
        }

        private void FractalPanel_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.LightGray);
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            var state = e.Graphics.Save();

            e.Graphics.TranslateTransform(fractalOffset.X, fractalOffset.Y);
            e.Graphics.ScaleTransform((float)fractalScale, (float)fractalScale);

            if (root != null)
            {
                if (showAllLevels)
                {
                    for (int level = currentLevel; level >= 0; --level)
                        DrawFractalLevel(e.Graphics, root, level, true);
                    for (int level = currentLevel; level >= 0; --level)
                        DrawFractalLevel(e.Graphics, root, level, false);
                }
                else
                {
                    DrawFractalLevel(e.Graphics, root, currentLevel, true);
                    DrawFractalLevel(e.Graphics, root, currentLevel, false);
                }
            }

            e.Graphics.Restore(state);
        }

        private void DrawFractalLevel(Graphics g, FractalNode node, int targetLevel, bool drawWhite)
        {
            foreach (var child in node.Children)
                DrawFractalLevel(g, child, targetLevel, drawWhite);

            if (node.Level == targetLevel)
            {
                float screenSize = node.Size * (float)fractalScale;
                if (screenSize < 0.5f) return;

                PointF[] whitebox = node.WhiteBoxPoints;
                PointF[] blackbox = node.BlackBoxPoints;

                if (drawWhite)
                {
                    Color whiteboxColor = Color.White;

                    using (Brush whiteboxBrush = new SolidBrush(whiteboxColor))
                        g.FillPolygon(whiteboxBrush, whitebox);
                }
                else 
                {
                    Color blackboxColor = Color.Black;

                    using (Brush blackboxBrush = new SolidBrush(blackboxColor))
                        g.FillPolygon(blackboxBrush, blackbox);
                }

                //float penWidth = Math.Max(0.1f, (showAllLevels ? 0.5f : 1f) / (float)fractalScale);
                //using (Pen pen = new Pen(Color.Black, penWidth))
                //{
                //    g.DrawPolygon(pen, whitebox);
                //    g.DrawPolygon(pen, blackbox);
                //}
            }
        }

        private void TreePanel_Paint(object sender, PaintEventArgs e)
        {
            e.Graphics.Clear(Color.White);
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;

            var state = e.Graphics.Save();

            e.Graphics.TranslateTransform(treeScrollOffset.X, treeScrollOffset.Y);
            e.Graphics.ScaleTransform((float)treeScale, (float)treeScale);

            if (root != null)
            {
                DrawTreeConnections(e.Graphics, root);
                DrawTreeNodes(e.Graphics, root);
            }

            e.Graphics.Restore(state);
        }

        private void DrawTreeConnections(Graphics g, FractalNode node)
        {
            foreach (var child in node.Children)
            {
                float penWidth = Math.Max(0.5f, 2f / (float)treeScale);
                using (var pen = new Pen(Color.Black, penWidth))
                    g.DrawLine(pen, node.TreeX, node.TreeY, child.TreeX, child.TreeY);
                DrawTreeConnections(g, child);
            }
        }

        private void DrawTreeNodes(Graphics g, FractalNode node)
        {
            Color color = node.Level % 2 == 0 ? Color.White: Color.Black;

            bool isSelected = showAllLevels ? node.Level <= currentLevel : node.Level == currentLevel;
            bool isHovered = node == hoveredNode;

            float baseNodeSize = isSelected ? 16 : 12;
            if (isHovered) baseNodeSize += 2;

            float nodeSize = baseNodeSize / (float)treeScale;

            using (var brush = new SolidBrush(color))
                g.FillEllipse(brush, node.TreeX - nodeSize / 2, node.TreeY - nodeSize / 2, nodeSize, nodeSize);

            Color borderColor = Color.Black;
            float borderWidth = 1f / (float)treeScale;

            if (isSelected)
            {
                borderColor = Color.Red;
                borderWidth = 3f / (float)treeScale;
            }
            else if (isHovered)
            {
                borderColor = Color.Blue;
                borderWidth = 2f / (float)treeScale;
            }

            using (var pen = new Pen(borderColor, borderWidth))
                g.DrawEllipse(pen, node.TreeX - nodeSize / 2, node.TreeY - nodeSize / 2, nodeSize, nodeSize);

            float fontSize = Math.Max(6f, 9f / (float)treeScale);
            using (var font = new Font("Arial", fontSize, FontStyle.Bold))
            using (var brush = new SolidBrush(Color.Black))
            {
                float textOffset = 10f / (float)treeScale;
                g.DrawString($"L{node.Level}", font, brush, node.TreeX + textOffset, node.TreeY - 6f / (float)treeScale);
            }

            foreach (var child in node.Children)
                DrawTreeNodes(g, child);
        }

        private void CalculateTreeLayout(FractalNode node, float x, float y, float xSpacing)
        {
            node.TreeX = x;
            node.TreeY = y;

            if (node.Children.Count > 0)
            {
                float totalWidth = xSpacing * (node.Children.Count - 1);
                float blackboxtX = x - totalWidth / 2f;

                for (int i = 0; i < node.Children.Count; i++)
                {
                    float childX = blackboxtX + i * xSpacing;
                    CalculateTreeLayout(node.Children[i], childX, y + 80, Math.Max(20, xSpacing / 1.2f));
                }
            }
        }
    }
}