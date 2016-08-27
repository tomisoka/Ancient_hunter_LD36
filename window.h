#pragma once
#include "common.h"

#define wHeight    720
#define wWidth    1024
#define wMinHeight 360
#define wMinWidth  512

class Window: public QWindow{
	Q_OBJECT
public:

signals:
	void exposeEv(QExposeEvent *ev);
	void focusInEv(QFocusEvent *ev);
	void focusOutEv(QFocusEvent *ev);
	void hideEv(QHideEvent *ev);
	void keyPressEv(QKeyEvent *ev);
	void keyReleaseEv(QKeyEvent *ev);
	void mouseDoubleClickEv(QMouseEvent *ev);
	void mouseMoveEv(QMouseEvent *ev);
	void mousePressEv(QMouseEvent *ev);
	void mouseReleaseEv(QMouseEvent *ev);
	void moveEv(QMoveEvent *ev);
	void resizeEv(QResizeEvent *ev);
	void showEv(QShowEvent *ev);
	void tabletEv(QTabletEvent *ev);
	void touchEv(QTouchEvent *ev);
	void wheelEv(QWheelEvent *ev);
protected:
	virtual void exposeEvent(QExposeEvent *ev){exposeEv(ev);}
	virtual void focusInEvent(QFocusEvent *ev){focusInEv(ev);}
	virtual void focusOutEvent(QFocusEvent *ev){focusOutEv(ev);}
	virtual void hideEvent(QHideEvent *ev){hideEv(ev);}
	virtual void keyPressEvent(QKeyEvent *ev){keyPressEv(ev);}
	virtual void keyReleaseEvent(QKeyEvent *ev){keyReleaseEv(ev);}
	virtual void mouseDoubleClickEvent(QMouseEvent *ev){mouseDoubleClickEv(ev);}
	virtual void mouseMoveEvent(QMouseEvent *ev){mouseMoveEv(ev);}
	virtual void mousePressEvent(QMouseEvent *ev){mousePressEv(ev);}
	virtual void mouseReleaseEvent(QMouseEvent *ev){mouseReleaseEv(ev);}
	virtual void moveEvent(QMoveEvent *ev){moveEv(ev);}
	virtual void resizeEvent(QResizeEvent *ev){resizeEv(ev);}
	virtual void showEvent(QShowEvent *ev){showEv(ev);}
	virtual void tabletEvent(QTabletEvent *ev){tabletEv(ev);}
	virtual void touchEvent(QTouchEvent *ev){touchEv(ev);}
	virtual void wheelEvent(QWheelEvent *ev){wheelEv(ev);}

};

class CloseEventFilter : public QObject{
	Q_OBJECT
public:
	CloseEventFilter(QObject *parent) : QObject(parent) {}

signals:
	void closed();
protected:
	bool eventFilter(QObject *obj, QEvent *event){
		if (event->type() == QEvent::Close){
			closed();
		}
		return QObject::eventFilter(obj, event);
	}
};

extern Window *window;
extern CloseEventFilter *closeFilter;

void setup_window();
void cleanup_window();
