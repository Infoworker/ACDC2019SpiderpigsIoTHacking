using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Azure.WebJobs.ServiceBus;
using Microsoft.Extensions.Logging;
using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace IoTHub_EventHub2
{
    public static class Function1
    {
        [FunctionName("Function1")]
        //public static void Run([EventHubTrigger("samples-workitems", Connection = "HostName=SpiderPigs-IoT-Hub.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=62326EMrXhsb61Ez5s4f8nbJYeTUs31FjgSM1U8Kh8A=")]string myEventHubMessage, TraceWriter log)
        //{
        //    log.Info($"C# Event Hub trigger function processed a message: {myEventHubMessage}");
        //}

        //public static async void Run(string myIoTHubMessage, ILogger log)
        //public static async void Run([EventHubTrigger("samples-workitems", Connection = "HostName=SpiderPigs-IoT-Hub.azure-devices.net;SharedAccessKeyName=iothubowner;SharedAccessKey=62326EMrXhsb61Ez5s4f8nbJYeTUs31FjgSM1U8Kh8A=")]string myIoTHubMessage, TraceWriter log)
        public static async void Run([EventHubTrigger("samples-workitems", Connection = "EventHubConnection")]string myIoTHubMessage, ILogger log)
        {
            var apiUrl = System.Environment.GetEnvironmentVariable("apiUrl");
            
            log.LogInformation($"C# IoT Hub trigger function processed a message: {myIoTHubMessage}");
            //Forward("test", apiUrl);

            using (var client = new HttpClient())
            {
                client.BaseAddress = new Uri("https://spiderpigs-covertops.azurewebsites.net/api/FlowChallanges/Issues");
                var result = await client.GetAsync("test");
                log.LogInformation(result.StatusCode.ToString());
            }
        }
    }
}
