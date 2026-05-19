using System;
using System.Collections.Generic;
using System.Windows;
using Rocketleague.ControllerOptimizer.ControllerUI;
using Rocketleague.ControllerOptimizer.Interop;

namespace Rocketleague.ControllerOptimizer.ControllerUI
{
    public partial class MainWindow : Window
    {
        private readonly ControllerManager _manager;
        private readonly ProfileManager _profileManager;

        public MainWindow()
        {
            InitializeComponent();
            _manager = new ControllerManager();
            _profileManager = new ProfileManager();
            _manager.Initialize();
            SettingsFrame.Content = new SettingsPanel();
            RefreshControllerList();
        }

        private void RefreshControllerList()
        {
            ControllerListBox.Items.Clear();
            var controllers = _manager.GetAvailableControllers();
            foreach (var controller in controllers)
            {
                ControllerListBox.Items.Add(controller.Name);
            }
        }

        private void RefreshControllersButton_Click(object sender, RoutedEventArgs e)
        {
            RefreshControllerList();
        }

        private void ConnectButton_Click(object sender, RoutedEventArgs e)
        {
            if (ControllerListBox.SelectedIndex < 0)
            {
                MessageBox.Show("Please select a controller.", "Connect", MessageBoxButton.OK, MessageBoxImage.Warning);
                return;
            }

            var success = _manager.Connect(ControllerListBox.SelectedIndex);
            ControllerStatusText.Text = success ? "Controller connected." : "Failed to connect to controller.";
        }

        private void LoadDefaultProfileButton_Click(object sender, RoutedEventArgs e)
        {
            var profile = _profileManager.CreateDefaultProfile();
            _manager.SetDeadzoneProfile(profile.DeadzoneConfig);
        }

        private void LoadCompetitiveProfileButton_Click(object sender, RoutedEventArgs e)
        {
            var profile = _profileManager.CreateRocketLeagueOptimizedProfile();
            _manager.SetDeadzoneProfile(profile.DeadzoneConfig);
        }
    }
}
