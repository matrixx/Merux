#ifndef MER_H
#define MER_H

#include <QGLWidget>
#include <QPixmap>
#include <QVector2D>
#include <QTime>
#include <QTimer>
#include <QFeedbackHapticsEffect>

QTM_USE_NAMESPACE

class QGLShaderProgram;
class QGLBuffer;
class QMouseEvent;

class Mer : public QGLWidget
{
    Q_OBJECT
public:
    explicit Mer(QWidget *parent = 0);
    ~Mer();
public slots:

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

private slots:
    void stopRipple();

private:
    bool initShaders();
    void initBuffers();
    void initTexture();
    void updateUniforms();

    QGLShaderProgram* m_programObject;
    QGLBuffer* m_vertexBuffer;
    QGLBuffer* m_indexBuffer;
    int m_indices;
    int m_vertexAttr;

    QTime m_rippleTime;
    QTimer m_rippleTimeOut;
    QTimer m_updateTimer;
    GLuint m_texture;
    QPointF m_mousePos;
    QVector2D m_curSize;
    bool m_rippleOn;
    QVector2D m_ripplePos;
    QFeedbackHapticsEffect m_touchEffect;
};

#endif // MER_H
