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
  static const int kWindowHeight = 700;

  static const int kBoardCanvasWidth = 1000;
  static const int kBoardCanvasHeight = 450;
  static const int kBoardCanvasMargin = 100;
  static const int kBoardCanvasHeightOffset = 250;

  static const int kInfoCanvasWidth = 1000;
  static const int kInfoCanvasHeight = 200;
  static const int kInfoCanvasWidthMargin = 150;
  static const int kInfoCanvasHeightMargin = 0;
  static const int kInfoCanvasHeightOffset = 50;

  static const int kInfoLineNum = 6;

  static const int kBoardLineWidth = 5;
  static const int kInfoLineWidth = 2;

  float x_offet_for_better_display_;

  Board & ref_my_board_;
  ImagineBoard & ref_enemy_board_;

  void RenderGameUi(){
    ClearCanvas();
    RenderMyBoard();
    RenderEnemyBoard();
    RenderMyInfo();
    RenderEnemyInfo();
  }


  void ClearCanvas(){
    glClearColor ( 1.0f, 1.0f, 1.0f, 1.0f );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void RenderMyInfo(){
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(kInfoLineWidth);
    float center_x = kInfoCanvasWidth / 4;
    float center_y = kInfoCanvasHeight / 2 + kInfoCanvasHeightOffset;
    float width = kInfoCanvasWidth / 2 - kInfoCanvasWidthMargin;
    float height = kInfoCanvasHeight - kInfoCanvasHeightMargin;

    RenderRectangleWithShade(center_x, center_y, width, height, 1, 1, 1);

    RenderMyLabel(center_x, center_y, width, height, 1);
    RenderMyShipInfo(center_x, center_y, width, height);
  }

  void RenderMyShipInfo(float info_center_x, float info_center_y, float info_width, float info_height){
    RenderString("carrier: " + std::to_string(ref_my_board_.carrier_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 2), 1);
    RenderString("battleship: " + std::to_string(ref_my_board_.battleship_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 3), 1);
    RenderString("cruiser: " + std::to_string(ref_my_board_.cruiser_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 4), 1);
    RenderString("destroyer: " + std::to_string(ref_my_board_.destroyer_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 5), 1);
  }

  void RenderMyLabel(float info_center_x, float info_center_y, float info_width, float info_height, size_t which_line){
    float height_per_line = info_height / kInfoLineNum;
    RenderString("my board", info_center_x, GetLineCenterY(info_center_y, info_height, 1), 1);
  }

  float GetLineCenterY(float info_center_y, float info_height, size_t which_line){
    float height_per_line = info_height / kInfoLineNum;
    return info_center_y - info_height / 2 + height_per_line / 2 + (kInfoLineNum - which_line) * height_per_line;
  }

  void RenderEnemyInfo(){
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(kInfoLineWidth);
    float center_x = kInfoCanvasWidth / 4 * 3;
    float center_y = kInfoCanvasHeight / 2 + kInfoCanvasHeightOffset;
    float width = kInfoCanvasWidth / 2 - kInfoCanvasWidthMargin;
    float height = kInfoCanvasHeight - kInfoCanvasHeightMargin;

    RenderRectangleWithShade(center_x, center_y, width, height, 1, 1, 1);

    RenderEnemyLabel(center_x, center_y, width, height, 1);
    RenderEnemyShipInfo(center_x, center_y, width, height);
  }

  // TODO: possible code dup
  void RenderEnemyShipInfo(float info_center_x, float info_center_y, float info_width, float info_height){
    RenderString("carrier: " + std::to_string(ref_enemy_board_.carrier_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 2), 1);
    RenderString("battleship: " + std::to_string(ref_enemy_board_.battleship_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 3), 1);
    RenderString("cruiser: " + std::to_string(ref_enemy_board_.cruiser_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 4), 1);
    RenderString("destroyer: " + std::to_string(ref_enemy_board_.destroyer_num_) + " alive", info_center_x, GetLineCenterY(info_center_y, info_height, 5), 1);
  }

  // TODO: possible code dup
  void RenderEnemyLabel(float info_center_x, float info_center_y, float info_width, float info_height, size_t which_line){
    float height_per_line = info_height / kInfoLineNum;
    RenderString("enemy board", info_center_x, GetLineCenterY(info_center_y, info_height, 1), 1);
  }

  void RenderEnemyBoard(){
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(kBoardLineWidth);
    float center_x = kBoardCanvasWidth / 4 * 3;
    float center_y = kBoardCanvasHeight / 2 + kBoardCanvasHeightOffset;
    float width = kBoardCanvasHeight - kBoardCanvasMargin;
    RenderBoardBase(center_x, center_y, width);
    RenderStatesEnemyBoard(center_x, center_y, width);
  }

  void RenderStatesEnemyBoard(float board_center_x, float board_center_y, float board_width){
    for(size_t i = 0; i < kDim * kDim; i++){
      size_t row = i / kDim;
      size_t col = i % kDim;
      const unsigned char & state = ref_enemy_board_.states_[i];
      RenderOneStateEnemyBoard(state, ColToCenterX(col, board_center_x, board_width), RowToCenterY(row, board_center_y, board_width));
    }
  }

  void RenderOneStateEnemyBoard(unsigned char state, float center_x, float center_y){
    if(state & Board::OCCUPIED){
      RenderString(std::string().append(1, 'o'),center_x + x_offet_for_better_display_, center_y, 1);
    }else if(state & Board::ATTACKED){
      // only if not occupied, then we check if attacked
      // because if a location in enemy board is occupied, it must be attacked.
      RenderString(std::string().append(1, 'x'),center_x + x_offet_for_better_display_, center_y, 1);
    }
  }


  void RenderMyBoard(){
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(kBoardLineWidth);
    float center_x = kBoardCanvasWidth / 4;
    float center_y = kBoardCanvasHeight / 2 + kBoardCanvasHeightOffset;
    float width = kBoardCanvasHeight - kBoardCanvasMargin;
    RenderBoardBase(center_x, center_y, width);

    RenderShipsMyBoard(center_x, center_y, width);
    RenderStatesMyBoard(center_x, center_y, width);
  }

  void RenderShipsMyBoard(float board_center_x, float board_center_y, float board_width){
    for(Ship ship : ref_my_board_.on_board_ships_){
      RenderShip(ship.GetType(), ship.GetHeadLoaction(), ship.GetDirection(), board_center_x, board_center_y, board_width);
    }
  }

  // TODO: width_per_square can be a data member to avoid repeated computation
  void RenderShip(ShipType type, size_t head_location, Direction direction, float board_center_x, float board_center_y, float board_width){
    float width_per_square = board_width / kBoardDim;
    size_t ship_size = GetSizeFromType(type);
    size_t row = head_location / kDim;
    size_t col = head_location % kDim;
    float ship_margin = width_per_square / 7;
    switch(direction){
      case Direction::kHorisontal:{
        float rec_center_y = RowToCenterY(row, board_center_y, board_width);
        float rec_center_x = ColToCenterX(col, board_center_x, board_width) + (ship_size - 1) * width_per_square / 2;
        RenderRectangleWithShade(rec_center_x, rec_center_y, ship_size * width_per_square - ship_margin, width_per_square - ship_margin, .85, .85, .85);
        break;
      }
      case Direction ::kVertical:{
        float rec_center_x = ColToCenterX(col, board_center_x, board_width);
        float rec_center_y = RowToCenterY(row, board_center_y, board_width) - (ship_size - 1) * width_per_square / 2;
        RenderRectangleWithShade(rec_center_x, rec_center_y, width_per_square - ship_margin, ship_size * width_per_square - ship_margin, .85, .85, .85);
        break;
      }
      default:{
        Logger("can not render kUndefined Direction");
        assert(false);
      }
    }
  }

  void RenderRectangleWithShade(float center_x, float center_y, float width, float height, float r, float g, float b){
    float left_top_x = center_x - width / 2;
    float left_top_y = center_y + height / 2;

    float left_bottom_x = center_x - width / 2;
    float left_bottom_y = center_y - height / 2;

    float right_top_x = center_x + width / 2;
    float right_top_y = center_y + height / 2;

    float right_bottom_x = center_x + width / 2;
    float right_bottom_y = center_y - height / 2;

    glColor3f (r, g, b);
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
      RenderOneStateMyBoard(state, ColToCenterX(col, board_center_x, board_width), RowToCenterY(row, board_center_y, board_width));
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

  void RenderOneStateMyBoard(unsigned char state, float center_x, float center_y){
    if(state & Board::ATTACKED){
      RenderString(std::string().append(1, 'x'),center_x + x_offet_for_better_display_, center_y, 1);
    }
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
