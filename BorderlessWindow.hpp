#pragma once

#include <QQuickWindow>

class BorderlessWindow : public QQuickWindow
{
    Q_OBJECT

public:
    explicit BorderlessWindow(QWindow *parent = nullptr);

protected:
    virtual bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    LRESULT hitTest(POINT point) const;
    void modifyWindowStyle();
};
