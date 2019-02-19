using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using winsdkfb;

namespace winsdkfb.CS.Tests
{
    class MockHttpClient : IHttpClient
    {
        public String ResponseData;

        public IAsyncOperation<string> DeleteTaskAsync(string path, IReadOnlyDictionary<String, Object> parameters)
        {
            return Task.Run(() =>
            {
                return ResponseData;
            }).AsAsyncOperation<string>();
        }

        public IAsyncOperation<string> GetTaskAsync(string path, IReadOnlyDictionary<String, Object> parameters)
        {
            return Task.Run(() =>
            {
                return ResponseData;
            }).AsAsyncOperation<string>();
        }

        public string ParametersToQueryString(IReadOnlyDictionary<String, Object> Parameters)
        {
            throw new NotImplementedException();
        }

        public IAsyncOperation<string> PostTaskAsync(string path, IReadOnlyDictionary<String, Object> parameters)
        {
            return Task.Run(() =>
            {
                return ResponseData;
            }).AsAsyncOperation<string>();
        }
    }
}
