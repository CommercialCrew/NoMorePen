// Copyright (c) Microsoft Corporation and Contributors.
// Licensed under the MIT License.

#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

using namespace winrt;
using namespace Microsoft::UI::Xaml;

struct winrt::Windows::UI::Color ColorP = winrt::Microsoft::UI::Colors::Green();

// To learn more about WinUI, the WinUI project structure,
// and more about our project templates, see: http://aka.ms/winui-project-info.

namespace winrt::NoMorePen::implementation
{
    MainWindow::MainWindow()
    {
        flag = false;
        px = 100;
        py = 100;
        mySize = 16;
        InitializeComponent();
    }

    int32_t MainWindow::MyProperty()
    {
        throw hresult_not_implemented();
    }

    void MainWindow::MyProperty(int32_t /* value */)
    {
        throw hresult_not_implemented();
    }
}


void winrt::NoMorePen::implementation::MainWindow::Slider_ValueChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Controls::Primitives::RangeBaseValueChangedEventArgs const& e)
{
    mySize = e.NewValue();
}


void winrt::NoMorePen::implementation::MainWindow::CanvasControl_PointerPressed(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = true;
}


void winrt::NoMorePen::implementation::MainWindow::CanvasControl_PointerReleased(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    flag = false;
    px = py = 0.0;
    vx.push_back(px);
    vy.push_back(py);
    col.push_back(ColorP);
    sizeP.push_back(mySize);
}


void winrt::NoMorePen::implementation::MainWindow::CanvasControl_PointerMoved(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    px = e.GetCurrentPoint(canvas).Position().X;
    py = e.GetCurrentPoint(canvas).Position().Y;

    if (flag) {
        vx.push_back(px);
        vy.push_back(py);
        col.push_back(ColorP);
        sizeP.push_back(mySize);
        canvas.Invalidate();
    }
}


void winrt::NoMorePen::implementation::MainWindow::CanvasControl_Draw(winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasControl const& sender, winrt::Microsoft::Graphics::Canvas::UI::Xaml::CanvasDrawEventArgs const& args)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    int n = vx.size();
    if (n <= 2) return;

    for (int i = 1; i < n; i++) {
        if (vx[i] == 0.0 && vy[i] == 0.0) {
            i++;
            continue;
        }

        args.DrawingSession().DrawLine(vx[i - 1], vy[i - 1], vx[i], vy[i], col[i], sizeP[i]);
        args.DrawingSession().FillCircle(vx[i - 1], vy[i - 1], sizeP[i] / 2, col[i]);
        args.DrawingSession().FillCircle(vx[i], vy[i], sizeP[i] / 2, col[i]);
    }
   // canvas.Invalidate();
}


void winrt::NoMorePen::implementation::MainWindow::ColorPicker_ColorChanged(winrt::Microsoft::UI::Xaml::Controls::ColorPicker const& sender, winrt::Microsoft::UI::Xaml::Controls::ColorChangedEventArgs const& args)
{
    ColorP = args.NewColor();
}


void winrt::NoMorePen::implementation::MainWindow::Write_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fw;
    errno_t err;
    err = fopen_s(&fw, "C:/Users/PC/source/repos/NoMorePen/x64/Debug/NoMorePen/AppX/Assets/pendata.txt", "w");

    if (err == 0) {
        MessageBox(NULL, L"The file has been opened\n", L"test", NULL);

        int num = vx.size();
        fprintf_s(fw, "%d\n", num);
        for (int i = 0; i < num; i++) {
            fprintf_s(fw, "%f %f %d %d %d %d %f\n", vx[i], vy[i], col[i].A, col[i].B, col[i].G, col[i].R, sizeP[i]);
        }
        fclose(fw);
       /*vx.clear();
        vy.clear();
        col.clear();
        sizeP.clear();
        */ 
    }
    else {
        MessageBox(NULL, L"An error occurred while trying to open file\n", L"test2", NULL);
    }
}


void winrt::NoMorePen::implementation::MainWindow::Read_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fr;
    errno_t err;
    err = fopen_s(&fr, "C:/Users/PC/source/repos/NoMorePen/x64/Debug/NoMorePen/AppX/Assets/pendata.txt", "r");
    if (err == 0) {
        // MessageBox(NULL, L"The file has been opened\n", L"test", NULL);
        vx.clear();
        vy.clear();
        col.clear();
        sizeP.clear();

        int num;

        float vx1, vy1, sizeP1;
        int colA, colB, colG, colR;

        fscanf_s(fr, "%d", &num);
        for (int i = 0; i < num; i++) {
            fscanf_s(fr, "%f %f %hhu %hhu %hhu %hhu %f", &vx1, &vy1, &colA, &colB, &colG, &colR, &sizeP1);
            vx.push_back(vx1);
            vy.push_back(vy1);
            ColorP.A = colA;
            ColorP.B = colB;
            ColorP.G = colG;
            ColorP.R = colR;
            col.push_back(ColorP);
            sizeP.push_back(sizeP1);
        }
        fclose(fr);
        // MessageBox(NULL, L"The file has been closed\n", L"test", NULL);
        CanvasControl_PointerReleased(NULL, NULL);
    }
    else {
        MessageBox(NULL, L"An error occurred while trying to open file\n", L"test2", NULL);
    }
}


void winrt::NoMorePen::implementation::MainWindow::Clear_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    vx.clear();
    vy.clear();
    col.clear();
    sizeP.clear();
    flag = false;
    px = 100;
    py = 100;
    mySize = 16;
}

void winrt::NoMorePen::implementation::MainWindow::MenuFlyoutItem_Click(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    vx.clear();
    vy.clear();
    sizeP.clear();
    col.clear();
}


void winrt::NoMorePen::implementation::MainWindow::MenuFlyoutItem_Click_1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/PC/source/repos/NoMorePen/x64/Debug/NoMorePen/AppX/Assets/pendata.txt", "w");

    if (fp) {
        int n = vx.size();
        for (int i = 0; i < n; i++) {
            fprintf(fp, "%f %f %d %d %d %d %f ", vx[i], vy[i], col[i].R, col[i].G, col[i].B, col[i].A, sizeP[i]);
        }
        fclose(fp);
    }
}


void winrt::NoMorePen::implementation::MainWindow::MenuFlyoutItem_Click_2(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    FILE* fp;
    fopen_s(&fp, "C:/Users/PC/source/repos/NoMorePen/x64/Debug/NoMorePen/AppX/Assets/pendata.txt", "r");

    if (fp) {
        vx.clear(); 
        vy.clear();
        sizeP.clear();
        col.clear();

        float x, y, s;
        int r, g, b, a;
        while (EOF != fscanf_s(fp, "%f %f %d %d %d %d %f", &x, &y, &r, &g, &b, &a, &s)) {
            vx.push_back(x);
            vy.push_back(y);
            Windows::UI::Color tCol;
            tCol.R = r;
            tCol.G = g;
            tCol.B = b;
            tCol.A = a;
            col.push_back(tCol);
            sizeP.push_back(s);
        }
        fclose(fp);
    }
}


void winrt::NoMorePen::implementation::MainWindow::MenuFlyoutItem_Click_3(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::RoutedEventArgs const& e)
{
    this->Close();
}


void winrt::NoMorePen::implementation::MainWindow::CanvasControl_PointerEntered_1(winrt::Windows::Foundation::IInspectable const& sender, winrt::Microsoft::UI::Xaml::Input::PointerRoutedEventArgs const& e)
{
    CanvasControl canvas = sender.as<CanvasControl>();
    canvas.Invalidate();
}
