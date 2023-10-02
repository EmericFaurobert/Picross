#pragma once

#include <QPushButton>
#include <QMouseEvent>

class QRightClickableButton : public QPushButton
{
	Q_OBJECT

	private slots:
		void mousePressEvent(QMouseEvent *e)
		{
			if (e->button() == Qt::LeftButton)
				emit leftClicked();

			if (e->button() == Qt::RightButton)
				emit rightClicked();
		}

	signals:
		void leftClicked();
		void rightClicked();
};