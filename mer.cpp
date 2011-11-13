#include "mer.h"
#include <QGLShaderProgram>
#include <QGLBuffer>
#include <QApplication>
#include <QMouseEvent>
#include <QPointF>
#include <QDebug>

Mer::Mer(QWidget *parent) :
    QGLWidget(parent), m_indices(0), m_mousePos(QPointF()), m_rippleOn(false), m_ripplePos(QVector2D(0, 0))
{
    m_curSize = QVector2D(width(), height());
    m_touchEffect.setIntensity(1.0);
    m_touchEffect.setDuration(100);
    m_touchEffect.setFadeTime(700);
    m_touchEffect.setFadeIntensity(0.0);
    m_rippleTimeOut.setInterval(1000);
    connect(&m_rippleTimeOut, SIGNAL(timeout()), this, SLOT(stopRipple()));
    m_updateTimer.setInterval(16);
    connect(&m_updateTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
    m_updateTimer.start();
}

Mer::~Mer()
{
    m_programObject->disableAttributeArray(m_vertexAttr);
}

bool Mer::initShaders()
{
    m_programObject = new QGLShaderProgram();

    bool vertexShaderCompiled = m_programObject->addShaderFromSourceFile(QGLShader::Vertex, ":/shaders/vertexshader.glsl");
    bool fragmentShaderCompiled = m_programObject->addShaderFromSourceFile(QGLShader::Fragment, ":/shaders/fragmentshader.glsl");

    if (vertexShaderCompiled && fragmentShaderCompiled) {
        bool linked = m_programObject->link();
        if (linked) {
            bool bound = m_programObject->bind();
            if (bound) {
                m_vertexAttr = m_programObject->attributeLocation("vertex");
                m_programObject->enableAttributeArray(m_vertexAttr);
                return true;
            } else {
                qDebug() << "shader not bound";
            }
        } else {
            qDebug() << "shader not linked";
        }
    } else {
        qDebug() << "one of shaders did not compile: vertex:" << vertexShaderCompiled << "fragment:" << fragmentShaderCompiled;
    }
    return false;
}

void Mer::initBuffers()
{
    qreal ratio = qMin(m_curSize.x(), m_curSize.y() / qMax(m_curSize.x(), m_curSize.y()));
    qreal xside = 150;
    qreal yside = xside * ratio;
    qreal xpiece = 2.00 / xside;
    qreal ypiece = 2.00 / yside;
    QVector<float> vertices;
    for (int y = 0; y < yside; ++y) {
        for (int x = 0; x < xside; ++x) {
            vertices << x * xpiece - 1.0 << y * ypiece - 1.0 << 0.0;
        }
    }

    m_vertexBuffer = new QGLBuffer(QGLBuffer::VertexBuffer);
    m_vertexBuffer->create();
    m_vertexBuffer->bind();
    m_vertexBuffer->setUsagePattern(QGLBuffer::StaticDraw);
    m_vertexBuffer->allocate(vertices.constData(), (vertices.size() * sizeof(float)));
    m_programObject->setAttributeBuffer(m_vertexAttr, GL_FLOAT, 0, 3, sizeof(float) * 3);

    QVector<short> indices;
    int mult = 0;
    for (int i = 1; i < yside; ++i) {
        mult = i * xside;
        for (int j = 0; j < xside - 1; ++j) {
            indices << j + mult - xside << j + mult + 1.0 << mult + j;
            indices << j + mult + 1.0 << j + mult - xside << j + mult - xside + 1;
        }
    }
    m_indices = indices.size();

    m_indexBuffer = new QGLBuffer(QGLBuffer::IndexBuffer);
    m_indexBuffer->create();
    m_indexBuffer->bind();
    m_indexBuffer->setUsagePattern(QGLBuffer::StaticDraw);
    m_indexBuffer->allocate(indices.constData(), (indices.size() * sizeof(short)));
    qDebug("buffers created");
}

void Mer::initTexture()
{
    glGenTextures(1, &m_texture);
    m_texture = bindTexture(QPixmap(QString(":/textures/texture_logo1024.jpg")));
    qDebug("texture initialized");
}

void Mer::updateUniforms()
{
    m_programObject->setUniformValue("tex", 0);
    m_programObject->setUniformValue("res", m_curSize);
    float time = m_rippleTime.elapsed() / 10;

    m_programObject->setUniformValue("time", GLfloat(time));
    m_programObject->setUniformValue("rippleOn", m_rippleOn);
    m_programObject->setUniformValue("ripplePos", m_ripplePos);
}

void Mer::initializeGL()
{
#if QT_VERSION >= 0x040800
    initializeGLFunctions();
#endif
    QGLWidget::initializeGL();
    glViewport(0, 0, width(), height());

    glClearColor(0.0, 0.0, 0.0, 1.0);
    glDisable(GL_DEPTH_TEST);

    bool shadersInitialized = initShaders();
    if (shadersInitialized) {
        qDebug() << "success";
    } else {
        qDebug() << "failure, exiting";
        delete m_programObject;
        QApplication::quit();
    }
    initBuffers();
    initTexture();
}

void Mer::paintGL()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_programObject->bind();
    updateUniforms();

    glDrawElements(GL_TRIANGLES, m_indices, GL_UNSIGNED_SHORT, 0);
    m_programObject->release();
}

void Mer::mousePressEvent(QMouseEvent* event)
{
    m_mousePos = event->posF();
}

void Mer::mouseReleaseEvent(QMouseEvent* event)
{
    QPointF pos = event->pos();
    if (qAbs(pos.x() - m_mousePos.x()) < 100 && qAbs(pos.y() - m_mousePos.y()) < 100) {
        m_rippleOn = true;
        m_ripplePos = QVector2D(pos.x() * (2.0 / m_curSize.x()) - 1.0, -(pos.y() * (2.0 / m_curSize.y()) - 1.0));
        m_rippleTime.restart();
        m_rippleTimeOut.setSingleShot(true);
        m_rippleTimeOut.start();
        m_touchEffect.start();
    }
}

void Mer::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    m_curSize = QVector2D(w, h);
}

void Mer::stopRipple()
{
    m_rippleOn = false;
}



