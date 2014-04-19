#ifndef MDRENDERER_H
#define MDRENDERER_H
#include <QString>
extern "C" {
#include "hoedown/src/buffer.h"
#include <stdint.h>
}
class mdRenderer
{
public:
    mdRenderer();
    mdRenderer(QString text);
    void render(const QString markdownText);

//    char* u2c(const uint8_t* in,uint len);
//    uint8_t* c2u(const char* in,uint len);
    bool setStyle(QString css);
    QString read();
private:
    QString style;
    QString html;
//    hoedown_buffer *ob;
};

#endif // MDRENDERER_H
