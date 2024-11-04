#include <QFileDialog>
#include <QGridLayout>
#include "Visualizer.h"
#include "STLReader.h"
#include "OBJReader.h"
#include "OBJWriter.h"
#include "STLWriter.h"
#include "DataWriter.h"

Visualizer::Visualizer(QWidget* parent)
    : QMainWindow(parent)
{
    setupUi();

    connect(loadFile, &QPushButton::clicked, this, &Visualizer::onLoadFileClick);
    connect(translate, &QPushButton::clicked, this, &Visualizer::onTranslateClick);
    connect(exportFile, &QPushButton::clicked, this, &Visualizer::onExportClick);
}

Visualizer::~Visualizer()
{
}

void Visualizer::setupUi()
{
    loadFile = new QPushButton("Load File", this);
    translate = new QPushButton("Translate", this);
    exportFile = new QPushButton("Export", this);
    openglWidgetInput = new OpenGlWidget(this);
    openglWidgetOutput = new OpenGlWidget(this);
    graphicsSynchronizer = new GraphicsSynchronizer(openglWidgetInput, openglWidgetOutput);

    QGridLayout* layout = new QGridLayout(this);
    layout->addWidget(loadFile, 0, 0);
    layout->addWidget(translate, 0, 1);
    layout->addWidget(openglWidgetInput, 1, 0);
    layout->addWidget(openglWidgetOutput, 1, 1, 1, 2);
    layout->addWidget(exportFile, 0, 2);

    QWidget* centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    centralWidget->setLayout(layout);
}

void Visualizer::onLoadFileClick()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Files (*.stl *.obj)"));

    if (!fileName.isEmpty())
    {
        inputFilePath = fileName;
        triangulation = readFile(inputFilePath);
        OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(triangulation);
        openglWidgetInput->setData(data);
    }
}

void Visualizer::onTranslateClick()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty()) return;

    QString exportFileName;

    exportFileName = dir + (inputFilePath.endsWith(".stl", Qt::CaseInsensitive) ? "/output.obj" : "/output.stl");
    writeFile(exportFileName, triangulation);
    outputTriangulation = readFile(exportFileName);

    OpenGlWidget::Data data = convertTrianglulationToGraphicsObject(outputTriangulation);
    openglWidgetOutput->setData(data);

    // Clean up temporary file
    QFile::remove(exportFileName);
}

void Visualizer::onExportClick()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", inputFilePath.endsWith(".stl", Qt::CaseInsensitive) ? tr("files (*.obj)") : tr("files (*.stl)"));
    if (!fileName.isEmpty())
    {
        writeFile(fileName, outputTriangulation);
    }
}

OpenGlWidget::Data Visualizer::convertTrianglulationToGraphicsObject(const Triangulation& inTriangulation)
{
    OpenGlWidget::Data data;
        for each (Triangle triangle in inTriangulation.Triangles)
        {
            for each (Point point in triangle.Points())
            {
                data.vertices.push_back(inTriangulation.UniqueNumbers[point.X()]);
                data.vertices.push_back(inTriangulation.UniqueNumbers[point.Y()]);
                data.vertices.push_back(inTriangulation.UniqueNumbers[point.Z()]);
            }
    
            Point normal = triangle.Normal();
    
            for (size_t i = 0; i < 3; i++)
            {
                data.normals.push_back(inTriangulation.UniqueNumbers[normal.X()]);
                data.normals.push_back(inTriangulation.UniqueNumbers[normal.Y()]);
                data.normals.push_back(inTriangulation.UniqueNumbers[normal.Z()]);
            }
        }
    
        return data;
}

Triangulation Visualizer::readFile(const QString &filePath)
{
    Triangulation triangulation;
    if (filePath.endsWith(".stl", Qt::CaseInsensitive)) 
    {
        STLReader reader;
        reader.read(filePath.toStdString(), triangulation);
    }else if (filePath.endsWith(".obj", Qt::CaseInsensitive)) 
    {
        OBJReader reader;
        reader.read(filePath.toStdString(), triangulation);
    }

    return triangulation;
}

void Visualizer::writeFile(const QString &filePath, const Triangulation& triangulation)
{
    if (filePath.endsWith(".stl", Qt::CaseInsensitive))
    {
        STLWriter writer;
        writer.Write(filePath.toStdString(), triangulation);
    }
    else if (filePath.endsWith(".obj", Qt::CaseInsensitive))
    {
        OBJWriter writer;
        writer.Write(filePath.toStdString(), triangulation);
    }
}
