//
// Created by Kong, Chuilian on 5/20/18.
//

#ifndef BATTLESHIP_GAME_GRAPHIC_UTILS_H
#define BATTLESHIP_GAME_GRAPHIC_UTILS_H

#include <string>
#include "graphic/graphic_common.h"

static void drawOnePoint(float x, float y){
  glPointSize(4);
  glBegin(GL_POINTS);
  glVertex3f(x, y, 0.0);
  glEnd();
}

static void DrawString(const std::string & s, int x, int y, float r, float g, float b) {
  int offset = (s.length() + 1) * 4;
  x = x - offset;
  y = y - 3;
  int startX = x;
  glColor3f(r, g, b);
  //glBegin(GL_POINTS);
  for (const char & c : s ){
    if (c == 'a') {
      for (int i = 0; i < 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + 4);
      }
      x += 8;
    } else if (c == 'b') {
      for (int i = 0; i < 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + i, y);
        drawOnePoint(x + i, y + 4);
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 7, y + 5);
      drawOnePoint(x + 7, y + 7);
      drawOnePoint(x + 7, y + 6);
      drawOnePoint(x + 7, y + 1);
      drawOnePoint(x + 7, y + 2);
      drawOnePoint(x + 7, y + 3);
      x += 8;
    } else if (c == 'c') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y);
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 6, y + 1);
      drawOnePoint(x + 6, y + 2);

      drawOnePoint(x + 6, y + 6);
      drawOnePoint(x + 6, y + 7);

      x += 8;
    } else if (c == 'd') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y);
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 6, y + 1);
      drawOnePoint(x + 6, y + 2);
      drawOnePoint(x + 6, y + 3);
      drawOnePoint(x + 6, y + 4);
      drawOnePoint(x + 6, y + 5);
      drawOnePoint(x + 6, y + 6);
      drawOnePoint(x + 6, y + 7);

      x += 8;
    } else if (c == 'e') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + i, y + 0);
        drawOnePoint(x + i, y + 8);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 4);
      }
      x += 8;
    } else if (c == 'f') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + i, y + 8);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 4);
      }
      x += 8;
    } else if (c == 'g') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y);
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 6, y + 1);
      drawOnePoint(x + 6, y + 2);
      drawOnePoint(x + 6, y + 3);
      drawOnePoint(x + 5, y + 3);
      drawOnePoint(x + 7, y + 3);

      drawOnePoint(x + 6, y + 6);
      drawOnePoint(x + 6, y + 7);

      x += 8;
    } else if (c == 'h') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 4);
      }
      x += 8;
    } else if (c == 'i') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 3, y + i);
      }
      for (int i = 1; i <= 5; i++) {
        drawOnePoint(x + i, y + 0);
        drawOnePoint(x + i, y + 8);
      }
      x += 7;
    } else if (c == 'j') {
      for (int i = 1; i <= 8; i++) {
        drawOnePoint(x + 6, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 0);
      }
      drawOnePoint(x + 1, y + 3);
      drawOnePoint(x + 1, y + 2);
      drawOnePoint(x + 1, y + 1);
      x += 8;
    } else if (c == 'k') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      drawOnePoint(x + 6, y + 8);
      drawOnePoint(x + 5, y + 7);
      drawOnePoint(x + 4, y + 6);
      drawOnePoint(x + 3, y + 5);
      drawOnePoint(x + 2, y + 4);
      drawOnePoint(x + 2, y + 3);
      drawOnePoint(x + 3, y + 4);
      drawOnePoint(x + 4, y + 3);
      drawOnePoint(x + 5, y + 2);
      drawOnePoint(x + 6, y + 1);
      drawOnePoint(x + 7, y);
      x += 8;
    } else if (c == 'l') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + i, y);
      }
      x += 7;
    } else if (c == 'm') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      drawOnePoint(x + 3, y + 6);
      drawOnePoint(x + 2, y + 7);
      drawOnePoint(x + 4, y + 5);

      drawOnePoint(x + 5, y + 6);
      drawOnePoint(x + 6, y + 7);
      drawOnePoint(x + 4, y + 5);
      x += 8;
    } else if (c == 'n') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      drawOnePoint(x + 2, y + 7);
      drawOnePoint(x + 2, y + 6);
      drawOnePoint(x + 3, y + 5);
      drawOnePoint(x + 4, y + 4);
      drawOnePoint(x + 5, y + 3);
      drawOnePoint(x + 6, y + 2);
      drawOnePoint(x + 6, y + 1);
      x += 8;
    } else if (c == 'o' || c == '0') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + 0);
      }
      x += 8;
    } else if (c == 'p') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + 4);
      }
      drawOnePoint(x + 6, y + 7);
      drawOnePoint(x + 6, y + 5);
      drawOnePoint(x + 6, y + 6);
      x += 8;
    } else if (c == 'q') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
        if (i != 1)
          drawOnePoint(x + 7, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 8);
        if (i != 6)
          drawOnePoint(x + i, y + 0);
      }
      drawOnePoint(x + 4, y + 3);
      drawOnePoint(x + 5, y + 2);
      drawOnePoint(x + 6, y + 1);
      drawOnePoint(x + 7, y);
      x += 8;
    } else if (c == 'r') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + 4);
      }
      drawOnePoint(x + 6, y + 7);
      drawOnePoint(x + 6, y + 5);
      drawOnePoint(x + 6, y + 6);

      drawOnePoint(x + 4, y + 3);
      drawOnePoint(x + 5, y + 2);
      drawOnePoint(x + 6, y + 1);
      drawOnePoint(x + 7, y);
      x += 8;
    } else if (c == 's') {
      for (int i = 2; i <= 7; i++) {
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 1, y + 7);
      drawOnePoint(x + 1, y + 6);
      drawOnePoint(x + 1, y + 5);
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 4);
        drawOnePoint(x + i, y);
      }
      drawOnePoint(x + 7, y + 3);
      drawOnePoint(x + 7, y + 2);
      drawOnePoint(x + 7, y + 1);
      drawOnePoint(x + 1, y + 1);
      drawOnePoint(x + 1, y + 2);
      x += 8;
    } else if (c == 't') {
      for (int i = 0; i <= 8; i++) {
        drawOnePoint(x + 4, y + i);
      }
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + i, y + 8);
      }
      x += 7;
    } else if (c == 'u') {
      for (int i = 1; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 0);
      }
      x += 8;
    } else if (c == 'v') {
      for (int i = 2; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 6, y + i);
      }
      drawOnePoint(x + 2, y + 1);
      drawOnePoint(x + 5, y + 1);
      drawOnePoint(x + 3, y);
      drawOnePoint(x + 4, y);
      x += 7;
    } else if (c == 'w') {
      for (int i = 1; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      drawOnePoint(x + 2, y);
      drawOnePoint(x + 3, y);
      drawOnePoint(x + 5, y);
      drawOnePoint(x + 6, y);
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + 4, y + i);
      }
      x += 8;
    } else if (c == 'x') {
      for (int i = 1; i <= 7; i++)
        drawOnePoint(x + i, y + i);
      for (int i = 7; i >= 1; i--)
        drawOnePoint(x + i, y + 8 - i);
      x += 8;
    } else if (c == 'y') {
      drawOnePoint(x + 4, y);
      drawOnePoint(x + 4, y + 1);
      drawOnePoint(x + 4, y + 2);
      drawOnePoint(x + 4, y + 3);
      drawOnePoint(x + 4, y + 4);

      drawOnePoint(x + 3, y + 5);
      drawOnePoint(x + 2, y + 6);
      drawOnePoint(x + 1, y + 7);
      drawOnePoint(x + 1, y + 8);

      drawOnePoint(x + 5, y + 5);
      drawOnePoint(x + 6, y + 6);
      drawOnePoint(x + 7, y + 7);
      drawOnePoint(x + 7, y + 8);
      x += 8;
    } else if (c == 'z') {
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + i, y);
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + i);
      }
      drawOnePoint(x + 6, y + 7);
      x += 8;
    } else if (c == '1') {
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y);
      }
      for (int i = 1; i <= 8; i++) {
        drawOnePoint(x + 4, y + i);
      }
      drawOnePoint(x + 3, y + 7);
      x += 8;
    } else if (c == '2') {
      for (int i = 1; i <= 6; i++) {
        drawOnePoint(x + i, y);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 1, y + 7);
      drawOnePoint(x + 1, y + 6);

      drawOnePoint(x + 6, y + 7);
      drawOnePoint(x + 6, y + 6);
      drawOnePoint(x + 6, y + 5);
      drawOnePoint(x + 5, y + 4);
      drawOnePoint(x + 4, y + 3);
      drawOnePoint(x + 3, y + 2);
      drawOnePoint(x + 2, y + 1);
      x += 8;
    } else if (c == '3') {
      for (int i = 1; i <= 5; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y);
      }
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 6, y + i);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 4);
      }
      x += 8;
    } else if (c == '4') {
      for (int i = 2; i <= 8; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 7; i++) {
        drawOnePoint(x + i, y + 1);
      }
      for (int i = 0; i <= 4; i++) {
        drawOnePoint(x + 4, y + i);
      }
      x += 8;
    } else if (c == '5') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + i, y + 8);
      }
      for (int i = 4; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
      }
      drawOnePoint(x + 1, y + 1);
      drawOnePoint(x + 2, y);
      drawOnePoint(x + 3, y);
      drawOnePoint(x + 4, y);
      drawOnePoint(x + 5, y);
      drawOnePoint(x + 6, y);

      drawOnePoint(x + 7, y + 1);
      drawOnePoint(x + 7, y + 2);
      drawOnePoint(x + 7, y + 3);

      drawOnePoint(x + 6, y + 4);
      drawOnePoint(x + 5, y + 4);
      drawOnePoint(x + 4, y + 4);
      drawOnePoint(x + 3, y + 4);
      drawOnePoint(x + 2, y + 4);
      x += 8;
    } else if (c == '6') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y);
      }
      for (int i = 2; i <= 5; i++) {
        drawOnePoint(x + i, y + 4);
        drawOnePoint(x + i, y + 8);
      }
      drawOnePoint(x + 7, y + 1);
      drawOnePoint(x + 7, y + 2);
      drawOnePoint(x + 7, y + 3);
      drawOnePoint(x + 6, y + 4);
      x += 8;
    } else if (c == '7') {
      for (int i = 0; i <= 7; i++)
        drawOnePoint(x + i, y + 8);
      drawOnePoint(x + 7, y + 7);
      drawOnePoint(x + 7, y + 6);

      drawOnePoint(x + 6, y + 5);
      drawOnePoint(x + 5, y + 4);
      drawOnePoint(x + 4, y + 3);
      drawOnePoint(x + 3, y + 2);
      drawOnePoint(x + 2, y + 1);
      drawOnePoint(x + 1, y);
      x += 8;
    } else if (c == '8') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
        drawOnePoint(x + 7, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + 0);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 4);
      }
      x += 8;
    } else if (c == '9') {
      for (int i = 1; i <= 7; i++) {
        drawOnePoint(x + 7, y + i);
      }
      for (int i = 5; i <= 7; i++) {
        drawOnePoint(x + 1, y + i);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 8);
        drawOnePoint(x + i, y + 0);
      }
      for (int i = 2; i <= 6; i++) {
        drawOnePoint(x + i, y + 4);
      }
      drawOnePoint(x + 1, y + 0);
      x += 8;
    } else if (c == '.') {
      drawOnePoint(x + 1, y);
      x += 2;
    } else if (c == ',') {
      drawOnePoint(x + 1, y);
      drawOnePoint(x + 1, y + 1);
      x += 2;
    } else if (c == '\n') {
      y -= 10;
      x = startX;
    } else if (c == ' ') {
      x += 8;
    }
    x += 2;
  }
  //glEnd();
}

static void DrawString(const std::string & s, int x, int y) {
  DrawString(s, x, y, 0, 0, 0);
}


#endif //BATTLESHIP_GAME_GRAPHIC_UTILS_H
