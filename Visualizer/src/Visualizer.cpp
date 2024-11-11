#include <QFileDialog>
#include <QGridLayout>
#include <QStatusBar>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "Transformation.h"


Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();

    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
    connect(scale, &QPushButton::clicked, this, &Visualizer::onScaleClick);
    connect(rotate, &QPushButton::clicked, this, &Visualizer::onRotateClick);
    connect(distanceTranslation, &QPushButton::clicked, this, &Visualizer::onDistanceTranslateClick);
}

Visualizer::~Visualizer()
{

}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile = new QPushButton("Export", this);
    scale = new QPushButton("Scale", this);
    rotate = new QPushButton("Rotate", this);
    distanceTranslation = new QPushButton("Distance Translate", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);
    progressbar = new QProgressBar(this);
    customStatusBar = new QStatusBar(this);
    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);

    layout->addWidget(loadFile, 0, 0, 1, 2);
    layout->addWidget(translate, 0, 2, 1, 2);
    layout->addWidget(exportFile, 0, 4, 1, 2);
    layout->addWidget(openglWidgetInput, 2, 0, 1, 3);
    layout->addWidget(openglWidgetOutput, 2, 3, 1, 3);
    layout->addWidget(progressbar, 3, 0, 1, 6);
    layout->addWidget(scale, 1, 0, 1, 2);
    layout->addWidget(rotate, 1, 2, 1, 2);
    layout->addWidget(customStatusBar, 4, 0, 1, 6);
    layout->addWidget(distanceTranslation, 1, 4, 1, 2);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
    customStatusBar->setFixedHeight(30);

    customStatusBar->showMessage("Welcome to VISUALIZER APP ! Load your file.");
}

void  Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open File"), "", tr("files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
        {
            loadSTLFile(inputFilePath, triangulation, openglWidgetInput);
        }
        else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
        {
            loadOBJFile(inputFilePath, triangulation, openglWidgetInput);
        }
    }
}

void Visualizer::onTranslateClick()
{
    QFileDialog dialog(this);

    dialog.setFileMode(QFileDialog::Directory);

    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
        "/home",
        QFileDialog::ShowDirsOnly
        | QFileDialog::DontResolveSymlinks);

    progressbar->setValue(0);


    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.obj";
        ObjWriter writer;
        writer.Write(exportFileName.toStdString(), triangulation, progressbar);



        loadOBJFile(exportFileName, outputTriangulation, openglWidgetOutput);
        QFile::remove(exportFileName);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString exportFileName = dir + "/output.stl";
        STLWriter writer;
        writer.Write(exportFileName.toStdString(), triangulation, progressbar);

        loadSTLFile(exportFileName, outputTriangulation, openglWidgetOutput);
        QFile::remove(exportFileName);
    }

}

void Visualizer::onExportClick()
{
    QFileDialog dialog(this);

    if (inputFilePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.obj)"));
        ObjWriter writer;
        writer.Write(fileName.toStdString(), outputTriangulation, progressbar);
    }
    else if (inputFilePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File"), "", tr("files (*.stl)"));
        STLWriter writer;
        writer.Write(fileName.toStdString(), outputTriangulation, progressbar);
    }

}

void Visualizer::onScaleClick()
{
    Transform::Transformation transformation;
    transformation.scale(triangulation, outputTriangulation, 9);
    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
    openglWidgetOutput->setData(data);
}

void Visualizer::onRotateClick()
{
    int axis[3] = { 1,0,0 };
    Transform::Transformation transformation;
    transformation.rotate(triangulation, outputTriangulation, 45 ,axis);

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
    openglWidgetOutput->setData(data);
}

void Visualizer::onDistanceTranslateClick()
{
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
{
    OpenGlWidget::Data data;
    int count = 1;
    for each (const Triangle & triangle in inTriangulation.Triangles)
    {
        for each (const Point & point in triangle.Points())
        {
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
            data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
        }

        const Point& normal = triangle.Normal();

        for (size_t i = 0; i < 3; i++)
        {
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
            data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
        }

        progressbar->setValue(count);
        count++;
    }

    return data;
}

void Visualizer::loadOBJFile(const QString& filepath, Triangulation& triangulation, OpenGlWidget* openglWidgetWindow)
{
    OBJReader objreader;
    objreader.read(filepath.toStdString(), triangulation);

    progressbar->setRange(0, triangulation.Triangles.size());

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
    openglWidgetWindow->setData(data);
}

void Visualizer::loadSTLFile(const QString& filepath, Triangulation& triangulation, OpenGlWidget* openglWidgetWindow)
{
    STLReader stlreader;
    stlreader.read(filepath.toStdString(), triangulation);

    progressbar->setRange(0, triangulation.Triangles.size());

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
    openglWidgetWindow->setData(data);
}
