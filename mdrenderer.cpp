#include "mdrenderer.h"
extern "C" {
#include "hoedown/src/html.h"
#include "hoedown/src/markdown.h"
#include "hoedown/src/buffer.h"
}
#include <QString>
#include <QFile>
#include <string>
#include <malloc.h>

#define OUTPUT_UNIT 1024

mdRenderer::mdRenderer(QString text="")
{
    if(!text.isEmpty())
        render(text);
    setStyle("://styles/github.css");
}

mdRenderer::mdRenderer()
{
    setStyle("://styles/github.css");
}

void mdRenderer::render(const QString markdownText)
{
    if(markdownText.isEmpty())
    {
        html ="";
        return;
    }

    QByteArray utf8Data = markdownText.toUtf8();
    hoedown_buffer *doc = hoedown_buffer_new(utf8Data.length());
    hoedown_buffer_puts(doc, utf8Data.data());
    hoedown_buffer *out = hoedown_buffer_new(64);

    hoedown_renderer *renderer = hoedown_html_renderer_new(0, 16);
    hoedown_markdown *markdown = hoedown_markdown_new(0, 16, renderer);

    hoedown_markdown_render(out, doc->data, doc->size, markdown);

    hoedown_markdown_free(markdown);
    hoedown_html_renderer_free(renderer);

    html = QString::fromUtf8(hoedown_buffer_cstr(out));

    hoedown_buffer_free(out);

    html="<!DOCTYPE html> <html> <head> <style type=\"text/css\">"+
            style+
            "</style>"+
            "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">"+
            "</head> <body>"+
            html+
            "</body> </html>";
}

bool mdRenderer::setStyle(QString css)
{
    QFile file(css);
    if(!file.exists())
    {
        file.close();
        return false;
    }
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    style=file.readAll();
    file.close();
    return true;
}

QString mdRenderer::read()
{
    return html;
}

