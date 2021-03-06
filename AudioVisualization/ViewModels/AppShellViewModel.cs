﻿using AudioVisualization.Navigation;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using AudioVisualization.Views;
using Windows.UI.Xaml.Controls;

namespace AudioVisualization.ViewModels
{
    /// <summary>
    /// The ViewModel for the AppShell.
    /// </summary>
    class AppShellViewModel : ViewModel
    {
        public AppShellViewModel()
        {
            NavigationBarMenuItems = GetNavigationBarMenuItems();

            BottomNavigationBarMenuItems = GetBottomNavigationBarMenuItems();

            OnPropertyChanged("NavigationBarMenuItems");
            OnPropertyChanged("BottomNavigationBarMenuItems");
        }

        private List<INavigationBarMenuItem> GetNavigationBarMenuItems()
        {
            var result = new List<INavigationBarMenuItem>();

            // Add navigation items here.
            result.Add(new NavigationBarMenuItem(typeof(NowPlayingPage), null, NavigationBarItemPosition.Top, "Now Playing", null, (char)Symbol.Play));
            result.Add(new NavigationBarMenuItem(typeof(LibraryPage), null, NavigationBarItemPosition.Top, "Library", null, (char)Symbol.MusicInfo));

            return result;
        }

        private List<INavigationBarMenuItem> GetBottomNavigationBarMenuItems()
        {
            var result = new List<INavigationBarMenuItem>();

            // Add navigation items here.
#if DEBUG
            result.Add(new NavigationBarMenuItem(typeof(DebugPage), null, NavigationBarItemPosition.Bottom, "Debug", null, (char)Symbol.Admin));
#endif

            return result;
        }

        /// <summary>
        /// The navigation bar items at the bottom.
        /// </summary>
        public List<INavigationBarMenuItem> BottomNavigationBarMenuItems { get; }

        /// <summary>
        /// The navigation bar items at the top.
        /// </summary>
        public List<INavigationBarMenuItem> NavigationBarMenuItems { get; private set; }
    }
}
