#include "MainWindow.h"
#include "ConnectionWidget.h"
#include <QtWidgets>



///////////////////////////////////////////////////////////////////////////////
//  CONSTRUCTEUR
//  SETUP WIDGET
//
//  SLOT SET DATA
//  SLOT SET STATE
//
//  SLOT SLIDER VALUE CHANGED
//  SLOT BUTTON 1 PRESSED
//  SLOT BUTTON 2 PRESSED
//  SLOT BUTTON 1 RELEASED
//  SLOT BUTTON 2 RELEASED
///////////////////////////////////////////////////////////////////////////////


// CONSTRUCTEUR ///////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
	this->setupWidget();
	this->setWindowTitle(QGuiApplication::applicationDisplayName());
	this->slotSetState(AbstractRemoteJoystickClient::State::NotConnected);
	
	QObject::connect(m_connectionWidget, &ConnectionWidget::connectionRequest, &m_client, &AbstractRemoteJoystickClient::slotConnect);
	
	QObject::connect(&m_client, &RemoteJoystickClient::signalSetData, this, &MainWindow::slotSetData);
	QObject::connect(&m_client, &AbstractRemoteJoystickClient::stateChanged, this, &MainWindow::slotSetState);
	QObject::connect(&m_client, &AbstractRemoteJoystickClient::error, m_connectionWidget, &ConnectionWidget::slotError);
	
	QObject::connect(m_slider, &QSlider::valueChanged, this, &MainWindow::slotSliderValueChanged);
	QObject::connect(m_button1, &QPushButton::pressed, this, &MainWindow::slotButton1Pressed);
	QObject::connect(m_button2, &QPushButton::pressed, this, &MainWindow::slotButton2Pressed);
	QObject::connect(m_button1, &QPushButton::released, this, &MainWindow::slotButton1Released);
	QObject::connect(m_button2, &QPushButton::released, this, &MainWindow::slotButton2Released);
}

// SETUP WIDGET ///////////////////////////////////////////////////////////////
void MainWindow::setupWidget()
{
	m_connectionWidget = new ConnectionWidget(this);
	
	m_box = new QGroupBox("Controls",this);
	m_layout2 = new QGridLayout(m_box);
	m_box->setLayout(m_layout2);
	m_slider = new QSlider(this);
	m_slider->setMinimum(-10000);
	m_slider->setMaximum(10000);
	m_slider->setOrientation(Qt::Horizontal);
	m_button1 = new QPushButton("Button 1",this);
	m_button2 = new QPushButton("Button 2",this);
	m_button3 = new QPushButton("Button 3",this);
	m_layout2->addWidget(m_slider,0,0,1,3);
	m_layout2->addWidget(m_button1,1,0,1,1);
	m_layout2->addWidget(m_button2,1,1,1,1);
	m_layout2->addWidget(m_button3,1,2,1,1);
	
	// add widgets in the layout
	m_mainLayout = new QVBoxLayout(this);
	m_mainLayout->addWidget(m_connectionWidget);
	m_mainLayout->addWidget(m_box);
}






// SLOT SET DATA //////////////////////////////////////////////////////////////
void MainWindow::slotSetData(const QString &prop, QVariant data)
{
	if (prop == "button3")
	{
		bool b = data.toBool();
		m_button3->setDown(b);
	}
}

// SLOT SET STATE /////////////////////////////////////////////////////////////
void MainWindow::slotSetState(AbstractRemoteJoystickClient::State s)
{
	if (s == AbstractRemoteJoystickClient::State::NotConnected || s == AbstractRemoteJoystickClient::State::Error)
	{
		m_connectionWidget->setDisconnected();
	}
	else if (s == AbstractRemoteJoystickClient::State::Connecting)
	{
		m_connectionWidget->setConnecting();
	}
	else if (s == AbstractRemoteJoystickClient::State::Connected)
	{
		m_connectionWidget->setConnected();
		
		// initialisation du slider
		float f = ((float)m_slider->value()) / 10000.0f;
		m_client.slotSendAxisInfo(0,f);
	}
}






// SLOT SLIDER VALUE CHANGED //////////////////////////////////////////////////
void MainWindow::slotSliderValueChanged(int value)
{
	float f = ((float)value) / 10000.0f;
	m_client.slotSendAxisInfo(0,f);
}

// SLOT BUTTON 1 PRESSED //////////////////////////////////////////////////////
void MainWindow::slotButton1Pressed()
{
	m_client.slotSendButtonInfo(0,true);
}

// SLOT BUTTON 2 PRESSED //////////////////////////////////////////////////////
void MainWindow::slotButton2Pressed()
{
	m_client.slotSendButtonInfo(1,true);
}

// SLOT BUTTON 1 RELEASED /////////////////////////////////////////////////////
void MainWindow::slotButton1Released()
{
	m_client.slotSendButtonInfo(0,false);
}

// SLOT BUTTON 2 RELEASED /////////////////////////////////////////////////////
void MainWindow::slotButton2Released()
{
	m_client.slotSendButtonInfo(1,false);
}


