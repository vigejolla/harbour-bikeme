#include <QDebug>

#include "staticdataprovider.h"

StaticDataProvider::StaticDataProvider()
{
    _jcdecauxApiKey = "";

    CityData lille("Lille", "V'Lille", "FR",
                   "http://vlille.fr/stations/xml-stations.aspx",
                   NULL, "http://vlille.fr/stations/xml-station.aspx?borne=${number}");
    _citiesData["Lille"] = lille;
    CityData nice("Nice", "Vélo Bleu", "FR",
                  "http://www.velobleu.org/cartoV2/libProxyCarto.asp",
                  "http://www.velobleu.org/cartoV2/libProxyCarto.asp", NULL);
    _citiesData["Nice"] = nice;
    CityData london("London", "Barclays Cycle Hire", "UK",
                    "http://www.tfl.gov.uk/tfl/syndication/feeds/cycle-hire/livecyclehireupdates.xml",
                    "http://www.tfl.gov.uk/tfl/syndication/feeds/cycle-hire/livecyclehireupdates.xml", NULL);
    _citiesData["London"] = london;
}

QHash<QString, CityData> StaticDataProvider::getCitiesData()
{
    return _citiesData;
}

QString StaticDataProvider::getUrlForJCDecauxContracts()
{
    // https://developer.jcdecaux.com/rest/vls/contracts
    return "https://api.jcdecaux.com/vls/v1/contracts?apiKey=" + _jcdecauxApiKey;
}

QString StaticDataProvider::getCartoUrl(QString city)
{
    if (_citiesData.contains(city)) {
        return _citiesData.value(city).getUrlCarto();
    }
    return "https://developer.jcdecaux.com/rest/vls/stations/" + city + ".json";
}

QString StaticDataProvider::getAllStationsDetailsUrl(QString city)
{
    if (_citiesData.contains(city)) {
        QString url = _citiesData.value(city).getUrlAllStationsDetails();
        qDebug() << "all stations for" << city << url;
        return url != NULL ? url.replace("${city}", city) : NULL;
    }
    return "https://api.jcdecaux.com/vls/v1/stations?contract=" + city + "&apiKey=" + _jcdecauxApiKey;
}

QString StaticDataProvider::getStationDetailsUrl(QString city, QString stationNumber)
{
    if (_citiesData.contains(city)) {
        QString url = _citiesData.value(city).getUrlStationDetails();
        qDebug() << "station details for " << city << url;
        return url != NULL ? url.replace("${city}", city).replace("${number}", stationNumber) : NULL;
    }
    return "https://api.jcdecaux.com/vls/v1/stations/" + stationNumber +
            "?contract=" + city + "&apiKey=" + _jcdecauxApiKey;
}

QString StaticDataProvider::getCopyright(QString city)
{
    if (city.compare("London") == 0) {
        return "Powered by TfL Open Data";
    }
    return "";
}
