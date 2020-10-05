#include <math.h>
static inline int cvRound(float value) {
	return (int)roundf(value);
}

#include "opencv2/core/core_c.h"
#include "opencv2/core/types_c.h"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/videoio/videoio_c.h"

int main(void) {
	cvNamedWindow("Test", CV_WINDOW_AUTOSIZE);
	CvCapture* capture = cvCreateCameraCapture(0);
	IplImage* frame;
	while(1) {
		frame = cvQueryFrame(capture);
		cvShowImage("Test", frame);
	}
	cvReleaseCapture(&capture);
	cvDestroyWindow( "Test");
	return 0;
}
