#include "BorderlessWindow.hpp"

#include <QMargins>

#include <Windowsx.h>
#include <Windows.h>
#include <errno.h>

#include <QDebug>

namespace {
    const auto AERO_STYLE = (WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CLIPCHILDREN);
}

BorderlessWindow::BorderlessWindow(QWindow *parent) : QQuickWindow(parent)
{
    modifyWindowStyle();
}

struct HitRegion
{
    RECT    bounds;
    LRESULT region;

    bool contains(const POINT& point) const
    {
        return (
                    (point.x >= bounds.left && point.x < bounds.right) &&
                    (point.y >= bounds.top  && point.y < bounds.bottom)
                );
    }
};

LRESULT BorderlessWindow::hitTest(POINT point) const
{
    const auto borderX = GetSystemMetrics(SM_CYFRAME);
    const auto borderY = GetSystemMetrics(SM_CYFRAME);

    const auto xPos = x();
    const auto yPos = y();
    RECT winrect{xPos, yPos, xPos+width(), yPos+height()};

    const auto centerRegionSpacing = 20;
    if ((point.x >= xPos + width() / 2 - centerRegionSpacing && point.x <= xPos + width() / 2 + centerRegionSpacing) &&
        (point.y >= yPos + height() / 2 - centerRegionSpacing && point.y <= yPos + height() / 2 + centerRegionSpacing))
    {
        return HTCAPTION;
    }

    const HitRegion hitregions[]
    {
        {
            { winrect.left, winrect.bottom - borderY, winrect.left + borderX, winrect.bottom }, HTBOTTOMLEFT
        },
        {
            { winrect.right - borderX, winrect.bottom - borderY, winrect.right, winrect.bottom }, HTBOTTOMRIGHT
        },
        {
            { winrect.left, winrect.top, winrect.left + borderX  , winrect.top + borderY }, HTTOPLEFT
        },
        {
            { winrect.right - borderX , winrect.top, winrect.right, winrect.top + borderY }, HTTOPRIGHT
        },
        {
            { winrect.left, winrect.top, winrect.left + borderX , winrect.bottom }, HTLEFT
        },
        {
            { winrect.right - borderX , winrect.top, winrect.right, winrect.bottom }, HTRIGHT
        },
        {
            { winrect.left, winrect.top, winrect.right, winrect.top + borderY }, HTTOP
        },
        {
            { winrect.left, winrect.bottom - borderY, winrect.right, winrect.bottom }, HTBOTTOM
        }
    };

    for (auto &&hr : hitregions)
    {
        if ( hr.contains(point) )
            return hr.region;
    }

    return HTCLIENT;
}

bool BorderlessWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG *msg = static_cast<MSG *>(message);

    switch (msg->message)
    {
        case WM_SHOWWINDOW:
        {
            modifyWindowStyle();

            return false;
        }
        break;

        case WM_NCCALCSIZE:
        {
            // This is what kills the border and extends the client rect to the size of the window rect.
            *result = 0;

            return true;
        }
        break;

        case WM_NCHITTEST:
        {
            // When we have no border or title bar, we need to perform our
            // own hit testing to allow resizing and moving.
            const POINT cursor {
                GET_X_LPARAM(msg->lParam),
                GET_Y_LPARAM(msg->lParam)
            };

            const auto ht = hitTest(cursor);

            if (ht != HTNOWHERE)
            {
                *result = ht;
                return true;
            }
        }
        break;
    }

    return QQuickWindow::nativeEvent(eventType, message, result);
}

void BorderlessWindow::modifyWindowStyle()
{
//    setFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::Window | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);

    const auto hwnd = reinterpret_cast<HWND>(winId());

    SetWindowLong(hwnd, GWL_STYLE, AERO_STYLE);

    auto extendedStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    extendedStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, extendedStyle);

    SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);
}
