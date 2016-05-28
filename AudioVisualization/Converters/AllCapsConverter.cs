﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.UI.Xaml.Data;

namespace AudioVisualization.Converters
{
    public class AllCapsConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, string language)
        {
            String content = value.ToString().Trim();

            return content.ToUpperInvariant();
        }

        public object ConvertBack(object value, Type targetType, object parameter, string language)
        {
            throw new NotImplementedException();
        }
    }
}