//
// Created by Kong, Chuilian on 5/20/18.
//

#ifndef GRAPTHIC_GAME_UI_H_
#define GRAPTHIC_GAME_UI_H_

#include <iostream>
#include "graphic/graphic_common.h"
#include "core/game/board.h"
#include "core/game/imagine_board.h"


class GameUi{
public:
  GameUi(Board& my_board, ImagineBoard& enemy_board):
    ref_my_board_(my_board),
    ref_enemy_board_(enemy_board){
  }

  int Run(){
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
      return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(kWindowWidth, kWindowHeight, "Battle Ship Game", NULL, NULL);
    if (!window)
    {
      glfwTerminate();
      return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval( 1 );

    // set up view
    //glViewport( 0, 0, 640, 480 );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    // see https://www.opengl.org/sdk/docs/man2/xhtml/glOrtho.xml
    glOrtho(0.0,kWindowWidth,0.0,kWindowHeight,0.0,1.0); // this creates a canvas you can do 2D drawing on

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      RenderGameUi();

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    glfwTerminate();
    return 0;
  }

private:
  static const size_t kBoardDim = 11;

  static const int kWindowWidth = 1000;
  static const int kWindowHeight = 500;
  static const int kWindowMargin = 150;

  static const int kBoardLineWidth = 5;

  float x_offet_for_better_display_;

  Board & ref_my_board_;
  ImagineBoard & ref_enemy_board_;

  void RenderGameUi(){
    ClearCanvas();
    RenderMyBoard();

    RenderRectangleWithShade(300, 300, 200, 100);
  }


  void ClearCanvas(){
    glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RenderMyBoard(){
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(kBoardLineWidth);
    float center_x = kWindowWidth / 4;
    float center_y = kWindowHeight / 2;
    float width = kWindowHeight - kWindowMargin;
    RenderBoardBase(center_x, center_y, width);

    RenderShipsMyBoard(center_x, center_y, width);
    RenderStatesMyBoard(center_x, center_y, width);
  }

  void RenderShipsMyBoard(float board_center_x, float board_center_y, float board_width){
    for(Ship ship : ref_my_board_.on_board_ships_){
      ship.GetHeadLoaction();
    }
  }

  void RenderShip(ShipType  type, size_t head_location, Direction direction, float board_center_x, float board_center_y, float board_width){
    size_t size = GetSizeFromType(type);
    size_t row = head_location / kDim;
    size_t col = head_location % kDim;
    switch(direction){
      case Direction::kHorisontal:{

        break;
      }
      case Direction ::kVertical:{
        break;
      }
      default:{
        Logger("can not render kNotAShip");
        assert(false);
      }
    }
  }

  void RenderRectangleWithShade(float center_x, float center_y, float width, float height){
    float left_top_x = center_x - width / 2;
    float left_top_y = center_y + height / 2;

    float left_bottom_x = center_x - width / 2;
    float left_bottom_y = center_y - height / 2;

    float right_top_x = center_x + width / 2;
    float right_top_y = center_y + height / 2;

    float right_bottom_x = center_x + width / 2;
    float right_bottom_y = center_y - height / 2;

    glColor3f (.85, .85, .85);
    glBegin(GL_POLYGON);
    glVertex3f(left_top_x, left_top_y, 0.0);
    glVertex3f(right_top_x, right_top_y, 0.0);
    glVertex3f(right_bottom_x, right_bottom_y, 0.0);
    glVertex3f(left_bottom_x, left_bottom_y, 0.0);
    glEnd();

    glColor3f (0.0, 0.0, 0.0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex3f(left_top_x, left_top_y, 0.0);
    glVertex3f(right_top_x, right_top_y, 0.0);
    glVertex3f(right_bottom_x, right_bottom_y, 0.0);
    glVertex3f(left_bottom_x, left_bottom_y, 0.0);
    glEnd();

    glLineWidth(kBoardLineWidth);
  }

  void RenderStatesMyBoard(float board_center_x, float board_center_y, float board_width){
    for(size_t i = 0; i < kDim * kDim; i++){
      size_t row = i / kDim;
      size_t col = i % kDim;
      const unsigned char & state = ref_my_board_.states_[i];
      RenderState(state, ColToCenterX(col, board_center_x, board_width), RowToCenterY(row, board_center_y, board_width));
    }
  }

  float RowToCenterY(size_t row, float board_center_y, float board_width){
    float width_per_square = board_width / kBoardDim;
    float center_bottom_y = board_center_y - board_width / 2 + width_per_square / 2;
    return center_bottom_y + (kDim -1 - row) * width_per_square;
  }

  float ColToCenterX(size_t col, float board_center_x, float board_width){
    float width_per_square = board_width / kBoardDim;
    float center_left_x = board_center_x - board_width / 2 + width_per_square / 2;
    return center_left_x + (col + 1) * width_per_square;
  }

  void RenderState(unsigned char state, float center_x, float center_y){
    if(state & Board::ATTACKED){
      RenderString(std::string().append(1, 'x'),center_x + x_offet_for_better_display_, center_y, 1);
    }else{
      RenderString(std::string().append(1, 'o'),center_x + x_offet_for_better_display_, center_y, 1);
    }
  }



  void RenderBaseBoards()
  {
    RenderBoardBase(kWindowWidth / 4, kWindowHeight / 2, kWindowHeight - kWindowMargin);
    RenderBoardBase(kWindowWidth / 4 * 3, kWindowHeight / 2, kWindowHeight - kWindowMargin);
  }

  void RenderSquare(float center_x, float center_y, float width){

    float left_top_x = center_x - width / 2;
    float left_top_y = center_y + width / 2;

    float left_bottom_x = center_x - width / 2;
    float left_bottom_y = center_y - width / 2;

    float right_top_x = center_x + width / 2;
    float right_top_y = center_y + width / 2;

    float right_bottom_x = center_x + width / 2;
    float right_bottom_y = center_y - width / 2;

    glBegin(GL_LINE_LOOP);
    glVertex3f(left_top_x, left_top_y, 0.0);
    glVertex3f(right_top_x, right_top_y, 0.0);
    glVertex3f(right_bottom_x, right_bottom_y, 0.0);
    glVertex3f(left_bottom_x, left_bottom_y, 0.0);
    glEnd();
  }

  void RenderBoardBase(float center_x, float center_y, float width){
    float width_per_square = width / kBoardDim;
    float center_left_bottom_x = center_x - width / 2 + width_per_square / 2;
    float center_left_bottom_y = center_y - width / 2 + width_per_square / 2;

    for(size_t i = 0; i < kBoardDim; i++){
      for(size_t j = 0; j < kBoardDim; j++){
        RenderSquare(center_left_bottom_x + i * width_per_square, center_left_bottom_y + j * width_per_square, width_per_square);
      }
    }

    x_offet_for_better_display_ = width_per_square / 7;

    for(size_t i = 0; i < kBoardDim - 1; i++){
      RenderString(std::to_string(kBoardDim - 1 - i), x_offet_for_better_display_ + center_left_bottom_x, center_left_bottom_y + i * width_per_square, 1);
    }

    for(size_t i = 0; i < kBoardDim - 1; i++){
      RenderString(std::string().append(1, 'a' + i), x_offet_for_better_display_ + center_left_bottom_x + (i + 1) * width_per_square, center_left_bottom_y + (kBoardDim - 1) * width_per_square , 1);
    }
  }

// TODO: need better method to render texts
  void RenderString(const std::string & s, float center_x, float center_y, float scale){
    DrawString(s, center_x, center_y);
  }
};

#endif //GRAPTHIC_GAME_UI_H_
