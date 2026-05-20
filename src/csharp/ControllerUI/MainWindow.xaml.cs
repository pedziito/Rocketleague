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
            LogDebug("Application started.");
            RefreshControllerList();
        }

        private void RefreshControllerList()
        {
            LogDebug("Refreshing controller list...");
            ControllerListBox.Items.Clear();
            var (controllers, rawJson) = _manager.GetAvailableControllers();
            LogDebug($"EnumerateControllers output: {rawJson}");
            LogDebug($"Parsed {controllers.Count} controller(s).");

            foreach (var controller in controllers)
            {
                ControllerListBox.Items.Add(controller.Name);
            }

            if (controllers.Count == 0)
            {
                LogDebug("No controllers detected. Verify the device is connected and drivers are installed.");
            }
        }

        private void LogDebug(string message)
        {
            var timestamp = DateTime.Now.ToString("HH:mm:ss");
            DebugTextBox.AppendText($"[{timestamp}] {message}\n");
            DebugTextBox.ScrollToEnd();
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
                LogDebug("Connect attempted with no controller selected.");
                return;
            }

            LogDebug($"Connecting to controller index {ControllerListBox.SelectedIndex}...");
            var success = _manager.Connect(ControllerListBox.SelectedIndex);
            ControllerStatusText.Text = success ? "Controller connected." : "Failed to connect to controller.";
            LogDebug(success ? "Connect succeeded." : "Connect failed.");
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
