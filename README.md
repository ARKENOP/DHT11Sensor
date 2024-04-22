## Application de Monitoring d'Humidité et de Température

### Objectif
L'objectif de cette application est de surveiller en temps réel les niveaux d'humidité et de température d'une pièce à l'aide d'un capteur connecté à un ESP32. Les données collectées sont ensuite envoyées à une feuille de calcul Google Sheets via une requête HTTP. De plus, une application .NET MAUI est utilisée pour récupérer et afficher les données sur différentes plateformes.

### Fonctionnalités principales
1. **Mesure en temps réel :** Le capteur DHT11 intégré à l'ESP32 mesure périodiquement la température et l'humidité de la pièce.
2. **Stockage local :** Les données sont stockées localement sur l'ESP32 en cas de perte de connexion Internet pour une durée maximale de 24 heures.
3. **Reconnexion automatique :** L'ESP32 tente automatiquement de se reconnecter au réseau WiFi lorsqu'une connexion est perdue.
4. **Envoi différé des données :** Les données stockées localement sont envoyées à la feuille de calcul Google Sheets dès que la connexion Internet est rétablie.
5. **Interface utilisateur simple :** L'application .NET MAUI dispose d'une interface utilisateur pour récupérer les données en temps réel et les afficher de manière conviviale sur différentes plateformes.

### Technologie utilisée
- **ESP32 :** Microcontrôleur utilisé pour lire les données du capteur et gérer la connexion WiFi.
- **Capteur DHT11 :** Capteur d'humidité et de température utilisé pour mesurer les conditions environnementales.
- **WiFi :** Module WiFi intégré à l'ESP32 pour la connectivité Internet.
- **HTTP :** Protocole utilisé pour envoyer les données à la feuille de calcul Google Sheets.
- **Google Sheets :** Feuille de calcul en ligne utilisée pour stocker et visualiser les données collectées.
- **.NET MAUI :** Framework multiplateforme pour le développement d'applications mobiles et de bureau.

### Configuration requise
- Un ESP32 avec un capteur DHT11.
- Accès à un réseau WiFi.
- Un compte Google pour accéder à Google Sheets.
- Installation de l'environnement de développement .NET MAUI pour la construction de l'application.

## Collaborateurs

- **Paul SUPIOT** - [@ARKENOP](https://github.com/ARKENOP)
- **Terry Barrillon** - [@Many0nne](https://github.com/Many0nne)
