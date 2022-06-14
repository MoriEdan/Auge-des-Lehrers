# Auge des Lehrers (ADL) - Virtuelles Auge auf Netzwerke

[![.github/workflows/build.yml](https://github.com/MoriEdan/Auge-des-Lehrers/actions/workflows/build.yml/badge.svg)](https://github.com/MoriEdan/Auge-des-Lehrers/actions/workflows/build.yml)
[![Neueste stabile Version](https://img.shields.io/github/release/veyon/veyon.svg?maxAge=3600)](https://github.com/MoriEdan/Auge-des-Lehrers/releases)
[![Gesamte Downloads auf Github](https://img.shields.io/github/downloads/veyon/veyon/total.svg?maxAge=3600)](https://github.com/MoriEdan/Auge-des-Lehrers/releases)
[![Dokumentationsstatus](https://readthedocs.org/projects/veyon/badge/?version=latest)](https://docs.adl.io/)
[![Auf Transifex lokalisieren](https://img.shields.io/badge/localise-on_transifex-green.svg)](https://www.transifex.com/adl-solutions/adl/)
[![license](https://img.shields.io/badge/license-GPLv2-green.svg)](LICENSE)


## Was ist ADL?

ADL ist eine kostenlose Open-Source-Software zur Überwachung und Steuerung
Computer auf mehreren Plattformen. ADL unterstützt Sie beim digitalen Unterrichten
Lernumgebungen, Durchführung virtueller Schulungen oder Fernunterstützung.

Die folgenden Funktionen sind in ADL verfügbar:

* Übersicht: Überwachen Sie alle Computer an einem oder mehreren Standorten oder Klassenzimmern
   * Fernzugriff: Anzeigen oder Steuern von Computern, um Benutzer zu beobachten und zu unterstützen
   * Demo: Übertragung des Lehrerbildschirms in Echtzeit (Vollbild/Fenster)
   * Bildschirmsperre: Machen Sie auf das aufmerksam, was gerade wichtig ist
   * Kommunikation: Senden Sie Textnachrichten an Schüler
   * Lektionen starten und beenden: Benutzer gleichzeitig ein- und ausloggen
   * Screenshots: Lernfortschritte festhalten und Verstöße dokumentieren
   * Programme & Websites: Starten Sie Programme und öffnen Sie Website-URLs aus der Ferne
   * Unterrichtsmaterial: Dokumente, Bilder und Videos einfach verteilen und öffnen
   * Verwaltung: Computer aus der Ferne ein-/ausschalten und neu starten


4.670 / 5.000
Çeviri sonuçları
## Lizenz

Copyright (c) 2004-2022 Tobias Junghans / Veyon Solutions.

Siehe die Datei COPYING für die GNU GENERAL PUBLIC LICENSE.


## Installation und Konfiguration

Weitere Informationen finden Sie im offiziellen ADL-Administratorhandbuch unter https://docs.veyon.io/en/latest/admin/index.html
für Informationen zur Installation und Konfiguration von ADL.


## Verwendungszweck

Bitte lesen Sie das offizielle ADL-Benutzerhandbuch unter https://docs.veyon.io/en/latest/user/index.html
für Informationen zur Verwendung von ADL.


## ADL unter Linux

### Herunterladen von Quellen

Holen Sie sich zuerst die neuesten Quellen, indem Sie das Git-Repository klonen und alle Submodule abrufen:

git clone --recursive https://github.com/MoriEdan/Auge-des-Lehrers.git && cd adl


### Abhängigkeiten installieren

Anforderungen für Debian-basierte Distributionen:

- Bauwerkzeuge: g++ libc6-dev make cmake dpkg-dev
- Qt5: qtbase5-dev qtbase5-private-dev qtbase5-dev-tools qttools5-dev qttools5-dev-tools qtdeclarative5-dev qtquickcontrols2-5-dev
- X11: xorg-dev libxtst-dev libfakekey-dev
- libjpeg: libjpeg-dev bereitgestellt von libjpeg-turbo8-dev oder libjpeg62-turbo-dev
- zlib: zlib1g-dev
- OpenSSL: libssl-dev
- PAM: libpam0g-dev
- procps: libprocps-dev
- LZO: liblzo2-dev
- QCA: libqca-qt5-2-dev
- LDAP: libldap2-dev
- SASL: libsasl2-dev

Als root können Sie ausführen

apt install g++ libc6-dev make cmake qtbase5-dev qtbase5-private-dev \
qtbase5-dev-tools qttools5-dev qttools5-dev-tools \
qtdeclarative5-dev qtquickcontrols2-5-dev libfakekey-dev \
xorg-dev libxtst-dev libjpeg-dev zlib1g-dev libssl-dev libpam0g-dev \
libprocps-dev liblzo2-dev libqca-qt5-2-dev libldap2-dev \
libsasl2-dev Ninja-Build



Anforderungen für RedHat-basierte Distributionen:

- Build-Tools: gcc-c++ make cmake rpm-build
- Qt5: qt5-devel qt5-qtbase-private-devel
- X11: libXtst-devel libXrandr-devel libXinerama-devel libXcursor-devel libXrandr-devel libXdamage-devel libXcomposite-devel libXfixes-devel libfakekey-devel
- libjpeg: libjpeg-turbo-devel
- zlib: zlib-devel
- OpenSSL: openssl-devel
- PAM: pam-devel
- procps: procps-devel
- LZO: lzo-devel
- QCA: qca-devel qca-qt5-devel
- LDAP: openldap-devel
- SASL: cyrus-sasl-devel

Als root können Sie ausführen

dnf install gcc-c++ make cmake rpm-build qt5-devel libXtst-devel libXrandr-devel libXinerama-devel libXcursor-devel \
             libXrandr-devel libXdamage-devel libXcomposite-devel libXfixes-devel libfakekey-devel libjpeg-turbo-devel zlib-devel \
             openssl-devel pam-devel procps-devel lzo-devel qca-devel qca-qt5-devel openldap-devel cyrus-sasl-devel ninja-build


### Konfigurieren und Erstellen von Quellen

Führen Sie die folgenden Befehle aus:

mkdir-Build
CD-Build
cm machen ..
mach -j4

HINWEIS: Wenn Sie ein .deb- oder .rpm-Paket für diese Software erstellen möchten, sollten Sie anstelle des bereitgestellten cmake-Befehls Folgendes verwenden:

cmake -DCMAKE_INSTALL_PREFIX=/usr ..

um Paketdateien in /usr statt in /usr/local zu installieren.

Wenn einige Anforderungen nicht erfüllt sind, wird CMake Sie darüber informieren und
Sie müssen die fehlende Software installieren, bevor Sie fortfahren können.

Sie können jetzt ein Paket generieren (.deb oder .rpm, je nachdem, in welchem ​​System Sie sich befinden).

Zum Generieren eines Pakets können Sie ausführen

fakeroot-Make-Paket

Dann erhalten Sie etwas wie adl_x.y.z_arch.deb oder adl-x.y.z.arch.rpm

Alternativ können Sie die erstellten Binärdateien direkt installieren (nicht empfohlen für
Produktionssysteme), indem Sie den folgenden Befehl als root ausführen:

Installation machen

### Arch-Linux

Ein PKGBUILD ist im [AUR](https://aur.archlinux.org/packages/adl/) zu finden.

### PPA

Dieses PPA enthält offizielle ADL-Pakete für Ubuntu, die sowohl für die Verwendung auf Desktop-Computern als auch für ARM-Boards (z. B. Raspberry Pi) geeignet sind. Obwohl nur Pakete für LTS-Releases verfügbar sind, sollten sie auch für nachfolgende Nicht-LTS-Releases funktionieren.

sudo add-apt-repository ppa:adl/stable
sudo apt-get update

### openSUSE

ADL ist im offiziellen openSUSE Tumbleweed-Repository verfügbar.

sudo zypper in adl

Verwenden Sie für openSUSE Leap 15.2 das inoffizielle Paket aus dem Education-Repository.

https://software.opensuse.org/package/adl?search_term=adl

## Nehmen Sie an der Entwicklung teil

Wenn Sie Interesse an ADL, seiner Programmierung, Grafik, Tests oder ähnlichem haben, können Sie sich gerne an der Entwicklung von ADL beteiligen!

Bevor Sie mit der Implementierung einer neuen Funktion beginnen, öffnen Sie bitte immer ein Problem unter https://github.com/MoriEdan/Auge-des-Lehrers/issues, um eine Diskussion über Ihre beabsichtigte Implementierung zu beginnen. Möglicherweise gibt es verschiedene Ideen, Verbesserungen, Hinweise oder vielleicht eine bereits laufende Arbeit an diesem Feature.


## Mehr Informationen

* https://adl.io/
* https://docs.adl.io/
* https://facebook.com/adl.io/
* https://twitter.com/adl_io
