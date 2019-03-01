using IoTHubTrigger = Microsoft.Azure.WebJobs.EventHubTriggerAttribute;

using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Azure.EventHubs;
using System.Text;
using System.Net.Http;
using Microsoft.Extensions.Logging;
using System;
using System.Threading;

namespace IoTHubEvents
{
    public static class Function1
    {
        private static HttpClient client = new HttpClient();

        [FunctionName("Function1")]
        public static async void RunAsync([IoTHubTrigger("messages/events", Connection = "EventHubConnection")]EventData message, ILogger log)
        {
            log.LogInformation($"C# IoT Hub trigger function processed a message: {Encoding.UTF8.GetString(message.Body.Array)}");

            using (var client = new HttpClient())
            {
                //Test connection to API
                client.BaseAddress = new Uri("https://spiderpigs-covertops.azurewebsites.net/api/FlowChallanges/Issues");
                var result = await client.GetAsync("");
                log.LogInformation("Connecting to API. Status: " + result.StatusCode.ToString());
                //log.LogInformation(result.Content.ReadAsStringAsync());
            }

            Thread.Sleep(500);
        }


    }
}