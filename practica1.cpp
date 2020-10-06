// g++ practica1.cpp -o testoutput -std=c++11 `pkg-config --cflags --libs opencv`

//1280×720 px
//1920×1080 px
//3840 x 2160 px

// todo a 852×480 px
#include <opencv2/highgui.hpp>
#include <bits/stdc++.h>
using namespace cv;
using namespace std;

//int arreglo[708][379];

// i(alto) y j(ancho) es la posicion del pixel de la imagen pequeña (480p)
// a y b es la resolucion de la imagen a reducir (a ancho, b alto)
// x y y es la posicion del pixel en la imagen grande
int resample(int alto, int ancho, int a, int b)
{
  int x = ancho * a / 852.0;
  int y = alto * b / 480.0;
  
  int index = (x + y * a) * 3;
  return index; // deberiamos de poner el pixel en la nueva imagen (en la posicion i,j)
}

int main()
{

  // formateo de la nueva imagen 480p

  //------------------
  cv::Mat image;
  image = cv::imread("test.jpg", cv::IMREAD_UNCHANGED); // Read the file
  cv::namedWindow("cvmat", cv::WINDOW_AUTOSIZE);        // Create a window for display.
  cv::imshow("cvmat", image);                           // Show our image inside it.

  // flatten the mat.
  uint totalElements = image.total() * image.channels(); // Note: image.total() == rows*cols.

  cv::Mat flat = image.reshape(1, totalElements); // 1xN mat of 1 channel, O(1) operation

  if (!image.isContinuous())
  {
    flat = flat.clone(); // O(N),
  }
  // flat.data is your array pointer
  auto *ptr = flat.data; // usually, its uchar*
  // You have your array, its length is flat.total() [rows=1, cols=totalElements]
  // Converting to vector
  std::vector<uchar> vec(flat.data, flat.data + flat.total());

  // angulo
  // int index = 0;
  // int avg = 0;
  // for (int i = 0; i < 720; i++)
  //   for (int j = 0; j < 1280; j++)
  //   {
  //     index = (j + i * 1280) * 3;
  //     avg = (flat.data[index] + flat.data[index + 1] + flat.data[index + 2]) / 3;
  //     flat.data[index] = avg;     //R
  //     flat.data[index + 1] = avg; //G
  //     flat.data[index + 2] = avg; //B
  //   }
  // angulo

  //--------------------------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------------------------

  // new image
  cv::Mat resampleImage(480, 852, CV_8UC3, Scalar(0, 0, 0));
  uint totalElementsResampleImage = resampleImage.total() * resampleImage.channels();
  cv::Mat flatResample = resampleImage.reshape(1, totalElementsResampleImage); // 1xN mat of 1 channel, O(1) operation

  if (!resampleImage.isContinuous())
  {
    flatResample = flatResample.clone(); // O(N),
  }

  auto *ptrResample = flatResample.data; 
  std::vector<uchar> vec1(flatResample.data, flatResample.data + flatResample.total());

  int index = 0;
  for (int i = 0; i < 480; i++)
    for (int j = 0; j < 852; j++)
    {
      index = (j + i * 852) * 3;

      int aux = resample(i, j, 1280, 720);

      flatResample.data[index] = flat.data[aux];
      flatResample.data[index + 1] = flat.data[aux + 1];
      flatResample.data[index + 2] = flat.data[aux + 2];
    }

  // fin new image
  //--------------------------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------------------------
  //--------------------------------------------------------------------------------------------------------------
  resampleImage = cv::Mat(480, 852, resampleImage.type(), ptrResample); // OR vec.data() instead of ptr
  cv::namedWindow("test", cv::WINDOW_AUTOSIZE);
  cv::imshow("test", resampleImage);
  // Testing by reconstruction of cvMat
  // cv::namedWindow("reconstructed", cv::WINDOW_AUTOSIZE);
  // cv::imshow("reconstructed", restored);
  cv::waitKey(0);
  return 0;
}
