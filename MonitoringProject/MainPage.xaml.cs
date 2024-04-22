using Google.Apis.Services; // Importe le namespace nécessaire pour accéder aux services Google API.
using Google.Apis.Sheets.v4; // Importe le namespace nécessaire pour accéder aux fonctionnalités de Google Sheets API.
using Google.Apis.Sheets.v4.Data; // Importe le namespace nécessaire pour accéder aux types de données de Google Sheets API.

namespace MonitoringProject
{
    public partial class MainPage : ContentPage
    {
        GoogleSheetsHelper sheetsHelper; // Déclare une variable pour stocker l'objet GoogleSheetsHelper.

        public MainPage()
        {
            InitializeComponent(); // Initialise la page.
            sheetsHelper = new GoogleSheetsHelper(); // Initialise une nouvelle instance de GoogleSheetsHelper.
        }
        
        private void OnNewButtonClicked(object sender, EventArgs e)
        {
            RetrieveAndDisplayData(); // Appelle la méthode pour récupérer et afficher les données.
        }

        private void RetrieveAndDisplayData()
        {
            if (sheetsHelper.Service != null) // Vérifie si le service Google Sheets est disponible.
            {
                string range = "logs!A2:E"; // Définit la plage de données à récupérer dans le tableau.

                try
                {
                    // Crée une demande pour récupérer les valeurs de la plage spécifiée dans le tableau.
                    SpreadsheetsResource.ValuesResource.GetRequest request =
                            sheetsHelper.Service.Spreadsheets.Values.Get(sheetsHelper.SpreadsheetId, range);

                    // Exécute la demande et récupère la réponse.
                    ValueRange response = request.Execute();

                    // Récupère les valeurs de la réponse.
                    IList<IList<object>> values = response.Values;

                    // Vérifie s'il y a des valeurs non nulles dans la réponse.
                    if (values != null && values.Count > 0)
                    {
                        var dataList = new List<DataItem>(); // Initialise une liste pour stocker les données.
                        foreach (var row in values) // Parcourt chaque ligne de valeurs.
                        {
                            if (row.Count >= 5) // Vérifie si la ligne contient au moins 5 valeurs.
                            {
                                // Crée un nouvel objet DataItem avec les valeurs de la ligne.
                                var item = new DataItem
                                {
                                    Date = DateTime.Parse(row[4].ToString()).ToString("dd/MM/yyyy HH:mm:ss"),
                                    Value1 = row[1].ToString(),
                                    Value2 = row[2].ToString(),
                                };
                                dataList.Add(item); // Ajoute l'objet à la liste des données.
                            }
                        }
                        DataListView.ItemsSource = dataList; // Définit la source de données de la ListView.
                    }
                    else
                    {
                        LabelData.Text = "Aucune donnée trouvée dans le tableur."; // Affiche un message s'il n'y a pas de données.
                    }
                }
                catch (Exception ex)
                {
                    LabelData.Text = $"Erreur lors de la récupération des données : {ex.Message}"; // Affiche un message d'erreur en cas d'échec.
                }
            }
            else
            {
                LabelData.Text = "Le service Google Sheets n'est pas disponible."; // Affiche un message si le service n'est pas disponible.
            }
        }
    }

    public class DataItem
    {
        public string Date { get; set; }
        public string Value1 { get; set; }
        public string Value2 { get; set; }
    }

    public class GoogleSheetsHelper
    {
        public SheetsService Service { get; set; } // Déclare une propriété pour stocker le service Google Sheets.
        public string SpreadsheetId { get; } = "1hqyGdObQ3ytBvJW59WVRAvODaIKJ66amkCL4oUVCCZA"; // Stocke l'identifiant du tableau.
        const string APPLICATION_NAME = "MonitoringProject"; // Stocke le nom de l'application.

        public GoogleSheetsHelper()
        {
            InitializeService(); // Initialise le service Google Sheets.
        }

        private void InitializeService()
        {
            try
            {
                // Crée le service Google Sheets en utilisant les informations d'authentification et l'API key.
                Service = new SheetsService(new BaseClientService.Initializer()
                {
                    ApiKey = "AIzaSyDr1yp-UmV7fAG9_2UimC3YuL5-Dd-ctig",
                    ApplicationName = APPLICATION_NAME,
                });
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Erreur lors de l'initialisation du service Google Sheets : {ex.Message}"); // Affiche un message d'erreur en cas d'échec.
                Service = null; // Définit le service à null en cas d'échec.
            }
        }
    }
}
