using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;

namespace Rocketleague.ControllerOptimizer.ControllerUI
{
    public class ProfileManager
    {
        private readonly string _profileDirectory;

        public ProfileManager()
        {
            _profileDirectory = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "profiles");
            Directory.CreateDirectory(_profileDirectory);
        }

        public class Profile
        {
            public string Name { get; set; }
            public Rocketleague.ControllerOptimizer.Interop.DeadzoneProfile DeadzoneConfig { get; set; }
            public int PollingRate { get; set; }
            public object DriftProfile { get; set; }
            public bool AntiDriftEnabled { get; set; }
            public DateTime CreatedAt { get; set; }
            public DateTime LastModified { get; set; }
        }

        public void SaveProfile(Profile profile)
        {
            profile.LastModified = DateTime.UtcNow;
            string path = Path.Combine(_profileDirectory, profile.Name + ".json");
            File.WriteAllText(path, JsonConvert.SerializeObject(profile, Formatting.Indented));
        }

        public Profile LoadProfile(string name)
        {
            string path = Path.Combine(_profileDirectory, name + ".json");
            if (!File.Exists(path)) return null;
            return JsonConvert.DeserializeObject<Profile>(File.ReadAllText(path));
        }

        public List<string> GetAllProfiles()
        {
            var profiles = new List<string>();
            foreach (var file in Directory.GetFiles(_profileDirectory, "*.json"))
            {
                profiles.Add(Path.GetFileNameWithoutExtension(file));
            }
            return profiles;
        }

        public void DeleteProfile(string name)
        {
            string path = Path.Combine(_profileDirectory, name + ".json");
            if (File.Exists(path)) File.Delete(path);
        }

        public Profile CreateDefaultProfile()
        {
            return new Profile
            {
                Name = "Default",
                DeadzoneConfig = new Rocketleague.ControllerOptimizer.Interop.DeadzoneProfile { Inner = 0.05f, Outer = 0.95f, Type = 0 },
                PollingRate = 500,
                AntiDriftEnabled = false,
                CreatedAt = DateTime.UtcNow,
                LastModified = DateTime.UtcNow
            };
        }

        public Profile CreateRocketLeagueOptimizedProfile()
        {
            return new Profile
            {
                Name = "Rocket League Competitive",
                DeadzoneConfig = new Rocketleague.ControllerOptimizer.Interop.DeadzoneProfile { Inner = 0.03f, Outer = 0.9f, Type = 1 },
                PollingRate = 1000,
                AntiDriftEnabled = true,
                CreatedAt = DateTime.UtcNow,
                LastModified = DateTime.UtcNow
            };
        }
    }
}
