using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

namespace UwpApplication2
{
    public sealed partial class MainPage : Page
    {
        private const string ADD = "+";
        private const string MUL = "*";
        private const string DIV = "/";
        private const string SUB = "-";
        private const string SQRT = "sqrt";
        private const string COS = "cos";
        private const string SIN = "sin";
        private const string TAN = "tan";
        private const string CTAN = "ctan";
        private const string LOG = "log";
        private const string POW = "^";
        
        private string action;

        public MainPage()
        {
            this.InitializeComponent();
        }

        private double FirstOperand()
        {
            try{
                double f = Convert.ToDouble(operand1.Text);
                Block.Text = "Operand 1";
                return f;
            }catch (Exception e){
                Block.Text = "Input error";
                action = "";
                return 0;
            }
        }

        private double SecondOperand()
        {
            try
            {
                double f = Convert.ToDouble(operand2.Text);
                TextBlock1.Text = "Operand 2";
                return f;
            }catch (Exception e){
                TextBlock1.Text = "Input error";
                action = "";
                return 0;
            }
        }

        private void CalculateClick(object sender, RoutedEventArgs e)
        {
            string act = action;
            switch (act)
            {
                case MUL:
                    res.Text = (Math.Round(FirstOperand() * SecondOperand(), 3)).ToString();
                    break;
                case ADD:
                    res.Text = (Math.Round(FirstOperand() + SecondOperand(), 3)).ToString();
                    break;
                case DIV:
                    res.Text = (Math.Round(FirstOperand() / SecondOperand(), 3)).ToString();
                    break;
                case SUB:
                    res.Text = (Math.Round(FirstOperand() - SecondOperand(), 3)).ToString();
                    break;
                case COS:
                    res.Text = (Math.Round(Math.Cos(FirstOperand()), 3)).ToString();
                    break;
                case SIN:
                    res.Text = (Math.Round(Math.Sin(FirstOperand()), 3)).ToString();
                    break;
                case TAN:
                    res.Text = (Math.Round(Math.Tan(FirstOperand()), 3)).ToString();
                    break;
                case CTAN:
                    res.Text = (Math.Round(Math.Atan(FirstOperand()), 3)).ToString();
                    break;
                case POW:
                    res.Text = Math.Round(Math.Pow(FirstOperand(), SecondOperand()), 3).ToString();
                    break;
                case SQRT:
                    res.Text = Math.Round(Math.Sqrt(FirstOperand()), 3).ToString();
                    break;
                case LOG:
                    res.Text = Math.Round(Math.Log(FirstOperand(), SecondOperand()), 3).ToString();
                    break;
                default:
                    res.Text = "Input error";
                    break;
            }
        }

        private void ChooseOperation(object sender, RoutedEventArgs e)
        {
            try{
                RadioButton rb = sender as RadioButton;
                action = (string) rb.Content;
            }catch (Exception er){
                action = "";
            }
        }

        private void AboutClick(object sender, RoutedEventArgs e)
        {
            about_me.Visibility = Visibility.Visible;
            Block.Visibility = Visibility.Collapsed;
            operand1.Visibility = Visibility.Collapsed;
            operand2.Visibility = Visibility.Collapsed;
            op4.Visibility = Visibility.Collapsed;
            op3.Visibility = Visibility.Collapsed;
            op2.Visibility = Visibility.Collapsed;
            op1.Visibility = Visibility.Collapsed;
            Button.Visibility = Visibility.Collapsed;
            TextBlock1.Visibility = Visibility.Collapsed;
            TextBlock2.Visibility = Visibility.Collapsed;
            res.Visibility = Visibility.Collapsed;
        }

        private void ArifmClick(object sender, RoutedEventArgs e)
        {
            op1.Content = ADD;
            op2.Content = SUB;
            op3.Content = DIV;
            op4.Content = MUL;
            op4.Visibility = Visibility.Visible;
        }

        private void TrigonomClick(object sender, RoutedEventArgs e)
        {
            op1.Content = SIN;
            op2.Content = COS;
            op3.Content = TAN;
            op4.Content = CTAN;
            op4.Visibility = Visibility.Visible;
        }

        private void AlgebrClick(object sender, RoutedEventArgs e)
        {
            op1.Content = POW;
            op2.Content = SQRT;
            op3.Content = LOG;
            op4.Visibility = Visibility.Collapsed;
        }
    }
}