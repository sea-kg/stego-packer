// place your code here

#include <QApplication>
#include <QString>
#include <QMap>
#include <QDir>
#include <QImage>
#include <QTime>
#include <QPainter>
#include <QFont>
#include <QList>
#include <QPaintDevice>

#include <iostream>
#include <math.h>


QString toBin(QString text)
{
	QByteArray arr = text.toAscii();
	QString result = "";
	for (int i = 0; i < arr.size(); i++) {
		unsigned char ch = arr.at(i);
		result += QString::number((int)ch, 2).rightJustified(8, '0');
	}
	result += QString::number(0, 2).rightJustified(8, '0');
	return result;
};
// ---------------------------------------------------------------------------

QString fromBin(QString text)
{
	QByteArray arr = text.toAscii();
	QByteArray arrRes( arr.size()/8, 0);
	
	for (int i = 0; i < arr.size(); i++) {
		char ch = arr.at(i);
		
		if (ch == '1') {
			int posBit = (7 - (i % 8));
			int posByte = i / 8;
			unsigned char resCh = arrRes.at(posByte);
			resCh = resCh + (0x00000001 << posBit);
			arrRes[posByte] = resCh;
		}
	}
	return QString(arrRes);
};

// ---------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	
	if (argc != 3 && argc != 5)
	{
		std::cout << "usage: \n\t" << argv[0] << " pack <inputfile.png> <outputfile.png> <text>\n or: \n";
		std::cout << "\t" << argv[0] << " unpack <file.png>\n\n";
		return -1;
	}
	QString command(argv[1]);
	
	if (command == "pack") {
		std::cout << " * PACK \n";
		QString inputfilename(argv[2]);
		QString outputfilename(argv[3]);
		QString text(argv[4]);
		QString textBin = toBin(text);
		
		QString res = text.toAscii().toHex();
		std::cout << "text: " << text.toStdString() << "\n";
		// std::cout << "textBin: " << textBin.toStdString() << "\n";
		// std::cout << "textLength: " << textBin.length() << "\n";
		
		QImage img;
		if(!img.load(inputfilename))
		{
			std::cout << "Could not load image\n";
			return -2;
		}
		
		int w = img.width();
		int h = img.height();
		
		std::cout << "size: " << w << "px X " << h << " px, possible write bytes: " << (h*w)/8 << "\n";

		QByteArray arr = textBin.toAscii();
		for (int i = 0; i < arr.size(); i++) {
			int x = i % w;
			int y = i / w;
			QRgb rgb = img.pixel(x,y);
			QColor color(rgb);
			int red = color.red();

			QString hex = QString::number( red, 2 ).rightJustified(8, '0');
			// std::cout << hex.toStdString() << " ";
			int new_red = red >> 1 << 1;
			if (arr.at(i) == '1')
				new_red += 1;
			hex = QString::number( new_red, 2 ).rightJustified(8, '0');
			
			// std::cout << hex.toStdString() << "\n";
			color.setRed(new_red);
			
			img.setPixel (x,y, color.rgb());
		}
		std::cout << "\n";
		img.save(outputfilename);
	} else if (command == "unpack") {
		std::cout << " * UNPACK \n";
		
		QString inputfilename(argv[2]);
		
		QImage img;
		if(!img.load(inputfilename))
		{
			std::cout << "Could not load image\n";
			return -2;
		}
		
		int w = img.width();
		int h = img.height();
		
		std::cout << "size: " << w << "px X " << h << " px, possible write bytes: " << (h*w)/8 << "\n";
		QString textBin = "";
		QString textLastByte = "";
		
		int count = w*h;
		
		
		for (int i = 0; i < count; i++) {
			int x = i % w;
			int y = i / w;		
			QRgb rgb = img.pixel(x,y);
			QColor color(rgb);
			int red = color.red();
			int b = red & 1;
			textBin += b == 0 ? "0" : "1";
			textLastByte += b == 0 ? "0" : "1";
			if (textLastByte.length() == 8)
			{
				if (textLastByte == "00000000")
				{
					break;
				}
				else
					textLastByte = "";
			}
		}
		// std::cout << "textBin: " << textBin.toStdString() << "\n";
		std::cout << "text: " << fromBin(textBin).toStdString() << "\n";
	} else {
		std::cout << "Unknown command\n";
	}
	
	return 0;
}
