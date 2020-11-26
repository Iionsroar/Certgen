#ifndef GLOBALS_H
#define GLOBALS_H

#include <QtGlobal>

QT_BEGIN_NAMESPACE
class QString;
QT_END_NAMESPACE

#define MAGIC_NUM 42

extern QString previewName;
extern QString templateFile;
extern qreal templateWidth;
extern qreal templateHeight;
extern QString fontFamily;
extern QString fontAlign;
extern qint32 fontSize;
extern qreal xPos;
extern qreal yPos;

#endif // GLOBALS_H
