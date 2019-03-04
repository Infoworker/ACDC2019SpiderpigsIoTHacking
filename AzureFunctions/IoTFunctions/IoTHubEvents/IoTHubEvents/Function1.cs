using IoTHubTrigger = Microsoft.Azure.WebJobs.EventHubTriggerAttribute;

using Microsoft.Azure.WebJobs;
using Microsoft.Azure.WebJobs.Host;
using Microsoft.Azure.EventHubs;
using System.Text;
using System.Net.Http;
using Microsoft.Extensions.Logging;
using System;
using System.Threading;
using Newtonsoft.Json;
using System.Dynamic;

namespace IoTHubEvents
{
    public static class Function1
    {
        private static HttpClient client = new HttpClient();

        [FunctionName("Function1")]
        public static async void RunAsync([IoTHubTrigger("messages/events", Connection = "EventHubConnection")]EventData message, ILogger log)
        {

            message.SystemProperties.TryGetValue("iothub-enqueuedtime", out var props);

            var date = props.ToString();

            log.LogInformation($"C# IoT Hub trigger function processed a message: {Encoding.UTF8.GetString(message.Body.Array)} on {date}");

            //var data = JSON.parse(message.Body.Array

            //var a = Encoding.UTF8.GetString(message.Properties.Keys);
            //var props = message.SystemProperties.TryGetValue;
            //string props = "";

            //var obj = new JavaScriptSerializer()

           

            dynamic o = new ExpandoObject();
            //o.temperature = messageObject.temp;

            try
            {
                var tempStr = Encoding.UTF8.GetString(message.Body.Array);
                var messageObject = JsonConvert.DeserializeObject<SensorData>(tempStr);

                messageObject.date = date;

                //Random cooling tower
                Random rnd = new Random();

                messageObject.location = "Cooling tower " + rnd.Next(1, 3); //"Cooling tower " + rnd.Next(1, 3);

                using (var client = new HttpClient())
                {

                    var conn = GetCustomConnectionString("APIEndpoint");
                    //Test connection to API
                    client.BaseAddress = new Uri(conn);

                    var jsoncont = JsonConvert.SerializeObject(messageObject);
                    var body = new StringContent(jsoncont, Encoding.UTF8, "application/json");

                    var result = await client.PostAsync("", body);

                    //var body = 

                    //await client.PostAsync("", );

                    log.LogInformation("Connecting to API. Status: " + result.StatusCode.ToString());
                    //log.LogInformation(result.Content.ReadAsStringAsync());
                }

            } catch (Exception e)
            {
                //Skip
                log.LogInformation(e.Message);
            }

            
        }

        public static string GetCustomConnectionString(string name)
        {
            string conStr = System.Environment.GetEnvironmentVariable($"ConnectionStrings:{name}", EnvironmentVariableTarget.Process);
            if (string.IsNullOrEmpty(conStr)) // Azure Functions App Service naming convention
                conStr = System.Environment.GetEnvironmentVariable($"CUSTOMCONNSTR_{name}", EnvironmentVariableTarget.Process);
            return conStr;
        }

    }

    class SensorData
    {
        public String location { get; set; }
        public double temperature { get; set; }
        public String date { get; set; }
    }

    
}