#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsItem>
#include "source/game/piece.h"
#include "source/game/pieceitem.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    cellSize = ui->boardView->width() / 8; // Размер клетки в 8 раз меньше размера доски

    boardScene = new QGraphicsScene(this); // Создаём сцену для доски. Привязываем её время жизни к окну (this)
    ui->boardView->setScene(boardScene); // boardView (QGraphicsView) будет визуализировать эту сцену

    // Выключаем прокрутку чтобы не мешала
    ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    ui->boardView->setFrameStyle(QFrame::NoFrame);

   // game.reset(new Chess::Game);
    game = new Chess::Game();
    game->RegisterObserver(*this);

    // Раскрашиваем доску
    PaintBoard();

   SetupFiguresWhite();
}

MainWindow::~MainWindow()
{
    // Внимание, item-ы не работают с принятой в Qt древовидной системой управления памятью!
    // Нужно удалить вручную!
    for (auto& item : boardScene->items())
        delete item;

    delete ui;
}

 void MainWindow::OnPieceAdded(Chess::Piece& Pos )
 {

     int X= Pos.get_pos().x_;
     int Y=Pos.get_pos().y_;
     QPointF top_left(X, Y);
     QPointF bottom_right(X + cellSize, Y + cellSize);
     // Будет еще реализованно в зависимости от type и color
     //выбор нужного  QString& fileName в классе PieceItem с помощью switch
     QString string;
     Chess::Color color_used = Pos.get_color();
     Chess::Type type_used=Pos.get_type();
     if (color_used==Chess::Color::White)
     {
                switch(type_used)
                {
                                case  Chess::Type::Pawn:
                    string = "D:/GitHub/Chess/graphics/pieces/pawn_white.png";
                    break;
                                case Chess::Type::Knight:
                     string = "D:/GitHub/Chess/graphics/pieces/knight_white.png";
                    break;
                                case Chess::Type::Bishop:
                      string = "D:/GitHub/Chess/graphics/pieces/bishop_white.png";
                      break;
                                case Chess::Type::Rook:
                      string = "D:/GitHub/Chess/graphics/pieces/rook_white.png";
                      break;
                                 case Chess::Type::Queen:
                      string = "D:/GitHub/Chess/graphics/pieces/queen_white.png";
                      break;
                                 case Chess::Type::King:
                      string = "D:/GitHub/Chess/graphics/pieces/king_white.png";
                      break;

     }
     }
     else
     {
         switch(type_used)
         {
                         case  Chess::Type::Pawn:
             string = "D:/GitHub/Chess/graphics/pieces/pawn_black.png";
             break;
                         case Chess::Type::Knight:
              string = "D:/GitHub/Chess/graphics/pieces/knight_black.png";
             break;
                         case Chess::Type::Bishop:
               string = "D:/GitHub/Chess/graphics/pieces/bishop_black.png";
               break;
                         case Chess::Type::Rook:
               string = "D:/GitHub/Chess/graphics/pieces/rook_black.png";
               break;
                          case Chess::Type::Queen:
               string = "D:/GitHub/Chess/graphics/pieces/queen_black.png";
               break;
                          case Chess::Type::King:
               string = "D:/GitHub/Chess/graphics/pieces/king_black.png";
               break;
     }
     }

     PieceItem *P = new PieceItem(game,string);
      P->scaled(cellSize, cellSize);
     auto item = boardScene->addPixmap(P->scaled(cellSize, cellSize));
     item->setPos(top_left);
    // delete item;



}
void MainWindow::PaintBoard()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            int x = i * cellSize;
            int y = j * cellSize;

            QPointF top_left(x, y);
            QPointF bottom_right(x + cellSize, y + cellSize);

            const QColor dark(68, 37, 33);
            const QColor light(231, 198, 114);

            QColor color = (i + j) % 2 ? dark : light; // Чередуем цвета квадратов

            QRectF rect(top_left, bottom_right);
            boardScene->addRect(rect, Qt::NoPen, QBrush(color)); // Добавляем квадраты на доску
        }
}

void MainWindow::SetupFiguresWhite()
{

    //setting black
    Chess::Piece * Rook_Black = new Chess::Piece(Chess::Type::Rook, Chess::Color::Black, Chess::Pos(0,0));
     game->AddPiece(*Rook_Black) ;

    Chess::Piece * Knight_Black = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black, Chess::Pos(cellSize,0));
    game->AddPiece(*Knight_Black) ;

    Chess::Piece * Bishop_black = new Chess::Piece(Chess::Type::Bishop, Chess::Color::Black, Chess::Pos(2*cellSize,0));
    game->AddPiece(*Bishop_black) ;

    Chess::Piece * King_black = new Chess::Piece(Chess::Type:: King, Chess::Color::Black, Chess::Pos(3*cellSize,0));
    game->AddPiece(*King_black) ;

    Chess::Piece * Quin_black = new Chess::Piece(Chess::Type:: Queen, Chess::Color::Black, Chess::Pos(4*cellSize,0));
    game->AddPiece(*Quin_black) ;

    Chess::Piece * Bishop_black2 = new Chess::Piece(Chess::Type::Bishop, Chess::Color::Black, Chess::Pos(5*cellSize,0));
    game->AddPiece(*Bishop_black2) ;

    Chess::Piece * Knight_Black2 = new Chess::Piece(Chess::Type::Knight, Chess::Color::Black, Chess::Pos(6*cellSize,0));
    game->AddPiece(*Knight_Black2) ;

    Chess::Piece * Rook_Black2 = new Chess::Piece(Chess::Type::Rook, Chess::Color::Black, Chess::Pos(7*cellSize,0));
     game->AddPiece(*Rook_Black2) ;

     Chess::Piece * Pawn_Black1 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(0,cellSize));
      game->AddPiece(*Pawn_Black1) ;

      Chess::Piece * Pawn_Black2 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(cellSize,cellSize));
       game->AddPiece(*Pawn_Black2) ;

       Chess::Piece * Pawn_Black3 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(2*cellSize,cellSize));
        game->AddPiece(*Pawn_Black3) ;

        Chess::Piece * Pawn_Black4 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(3*cellSize,cellSize));
         game->AddPiece(*Pawn_Black4) ;

         Chess::Piece * Pawn_Black5 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(4*cellSize,cellSize));
          game->AddPiece(*Pawn_Black5) ;

          Chess::Piece * Pawn_Black6 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(5*cellSize,cellSize));
           game->AddPiece(*Pawn_Black6) ;

           Chess::Piece * Pawn_Black7 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(6*cellSize,cellSize));
            game->AddPiece(*Pawn_Black7) ;

            Chess::Piece * Pawn_Black8 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::Black, Chess::Pos(7*cellSize,cellSize));
             game->AddPiece(*Pawn_Black8);

             //setting white

             Chess::Piece * Rook_White = new Chess::Piece(Chess::Type::Rook, Chess::Color::White, Chess::Pos(0,7*cellSize));
              game->AddPiece(*Rook_White) ;

             Chess::Piece * Knight_White = new Chess::Piece(Chess::Type::Knight, Chess::Color::White, Chess::Pos(cellSize,7*cellSize));
             game->AddPiece(*Knight_White) ;

             Chess::Piece * Bishop_White = new Chess::Piece(Chess::Type::Bishop, Chess::Color::White, Chess::Pos(2*cellSize,7*cellSize));
             game->AddPiece(*Bishop_White) ;

             Chess::Piece * Quin_White = new Chess::Piece(Chess::Type:: Queen, Chess::Color::White, Chess::Pos(3*cellSize,7*cellSize));
             game->AddPiece(*Quin_White) ;

             Chess::Piece * King_White = new Chess::Piece(Chess::Type:: King, Chess::Color::White, Chess::Pos(4*cellSize,7*cellSize));
             game->AddPiece(*King_White) ;

             Chess::Piece * Bishop_White2 = new Chess::Piece(Chess::Type::Bishop, Chess::Color::White, Chess::Pos(5*cellSize,7*cellSize));
             game->AddPiece(*Bishop_White2) ;

             Chess::Piece * Knight_White2 = new Chess::Piece(Chess::Type::Knight, Chess::Color::White, Chess::Pos(6*cellSize,7*cellSize));
             game->AddPiece(*Knight_White2) ;

             Chess::Piece * Rook_White2 = new Chess::Piece(Chess::Type::Rook, Chess::Color::White, Chess::Pos(7*cellSize,7*cellSize));
              game->AddPiece(*Rook_White2) ;

            Chess::Piece * Pawn_White1 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(0,6*cellSize));
               game->AddPiece(*Pawn_White1) ;

               Chess::Piece * Pawn_White2 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(cellSize,6*cellSize));
                game->AddPiece(*Pawn_White2) ;

                Chess::Piece * Pawn_White3 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(2*cellSize,6*cellSize));
                game->AddPiece(*Pawn_White3) ;

                 Chess::Piece * Pawn_White4 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(3*cellSize,6*cellSize));
                 game->AddPiece(*Pawn_White4) ;

                  Chess::Piece * Pawn_White5 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(4*cellSize,6*cellSize));
                  game->AddPiece(*Pawn_White5) ;

                   Chess::Piece * Pawn_White6 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(5*cellSize,6*cellSize));
                   game->AddPiece(*Pawn_White6) ;

                   Chess::Piece * Pawn_White7 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(6*cellSize,6*cellSize));
                     game->AddPiece(*Pawn_White7) ;

                    Chess::Piece * Pawn_White8 = new Chess::Piece(Chess::Type::Pawn, Chess::Color::White, Chess::Pos(7*cellSize,6*cellSize));
                      game->AddPiece(*Pawn_White8);









}













