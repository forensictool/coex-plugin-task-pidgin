#include "typeos.h"
#include <QSettings>

// ---------------------------------------------------------------------

QString TypeOS::platform() const {
	return "SomeOS";
};

// ---------------------------------------------------------------------

void TypeOS::setPlatform(QString) {};

// ---------------------------------------------------------------------

QString TypeOS::version() const {
	return "0";
};

// ---------------------------------------------------------------------

void TypeOS::setVersion(QString) {
	// nothing
};

// ---------------------------------------------------------------------

QString TypeOS::toString() const {
	return "SomeOS0";
};

// ---------------------------------------------------------------------
