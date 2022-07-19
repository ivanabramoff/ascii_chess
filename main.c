#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

uint8_t chess_board [8][8];
uint32_t move_count = 1;

#define PAWN            0x1
#define ROOK            0x2
#define KNIGHT          0x3
#define BISHOP          0x4
#define QUEEN           0x5
#define KING            0x6

#define WHITE           0x0
#define BLACK           0x8

#define CHECK           0x10

#define EN_PASSANT      0x20
#define PAWN_FIRST_MOVE 0x40

#define PIECE_TYPE_MASK 0x7


#define COLOR_TO_MOVE_LITERAL (move_count % 2 == 1) ? ("white") : ("black")
#define COLOR_TO_MOVE_NUMBER (move_count % 2 == 1) ? (WHITE) : (BLACK);


void init_board( void );
void draw_board( void );
void make_a_move(uint8_t src, uint8_t dest);

int8_t move_input(uint8_t *piece_no_modifiers, uint8_t *src, uint8_t *dest);
uint8_t piece = 0, src = 0, dest = 0;

int32_t main(int32_t argc, int8_t **argv)
{
    init_board();
    draw_board();
    
    move_input(&piece, &src, &dest);
    make_a_move(src, dest);
    draw_board();
    return 0;
}

void game_loop( void ) 
{

}

void init_board( void )
{
    chess_board[0][0] = ROOK | WHITE;
    chess_board[0][7] = ROOK | WHITE;
    chess_board[7][0] = ROOK | BLACK;
    chess_board[7][7] = ROOK | BLACK;

    chess_board[0][1] = KNIGHT | WHITE;
    chess_board[0][6] = KNIGHT | WHITE;
    chess_board[7][1] = KNIGHT | BLACK;
    chess_board[7][6] = KNIGHT | BLACK;

    chess_board[0][2] = BISHOP | WHITE;
    chess_board[0][5] = BISHOP | WHITE;
    chess_board[7][2] = BISHOP | BLACK;
    chess_board[7][5] = BISHOP | BLACK;

    chess_board[0][3] = QUEEN | WHITE;
    chess_board[7][3] = QUEEN | BLACK;

    chess_board[0][4] = KING | WHITE;
    chess_board[7][4] = KING | BLACK;

    for(int i = 0; i < 8; i++) {
        chess_board[1][i] = PAWN | WHITE | PAWN_FIRST_MOVE;
        chess_board[6][i] = PAWN | BLACK | PAWN_FIRST_MOVE;
    }
}

void draw_board( void ) 
{
    printf("\n");
    for(int i = 7; i >= 0; i--) {
        printf("\n");
        for(int j = 0; j < 8; j++) {
            int8_t piece; 
            switch(chess_board[i][j] & PIECE_TYPE_MASK) {
                case PAWN:
                    piece = 'P';
                    break;
                case ROOK:
                    piece = 'R';
                    break;
                case KNIGHT:
                    piece = 'N';
                    break;
                case BISHOP:
                    piece = 'B';
                    break;
                case QUEEN:
                    piece = 'Q';
                    break;
                case KING:
                    piece = 'K';
                    break;
                default:
                    piece = '*';
                    break;                    
            }

            if((chess_board[i][j] & BLACK) != 0)
                piece = tolower(piece); 

            printf("%c ", piece);
        }
    }

    printf("\n");
}

// makes a move, expects src and destination squares (from 0 to 63)
void make_a_move(uint8_t src, uint8_t dest) 
{
    chess_board[dest / 8][dest % 8] = chess_board[src / 8][src % 8];
    chess_board[src / 8][src % 8] = 0;
}

// move input, expects piece+src_square+dest_square
// for example, pd2d4 qa4d1 
int8_t move_input(uint8_t *piece_no_modifiers, uint8_t *src, uint8_t *dest) 
{
    printf("Make a move (%s to move): ", COLOR_TO_MOVE_LITERAL);
    char move[6];
    scanf("%s", &move);
    
    //checks if move makes sense
    if(! (('1' <= move[2] <= '8') && ('1' <= move[4] <= '8') && 
          ('a' <= move[1] <= 'h') && ('a' <= move[3] <= 'h') &&
        ((move[0] == 'p') || (move[0] == 'r') || (move[0] == 'n')  ||
         (move[0] == 'b') || (move[0] == 'q') || (move[0] == 'k')) )) 

        {
        printf("incorrect input!");
        return -1;
    }
    
    // build a piece and store in *piece_no_modifiers
    switch(tolower(move[0])) {
        case 'p':
            *piece_no_modifiers |= PAWN;
            break;
        case 'r':
            *piece_no_modifiers |= ROOK;
            break;
        case 'n':
            *piece_no_modifiers |= KNIGHT;
            break;
        case 'b':
            *piece_no_modifiers |= BISHOP;
            break;
        case 'q':
            *piece_no_modifiers |= QUEEN;
            break;
        case 'k':
            *piece_no_modifiers |= KING;
            break;
        default:
            *piece_no_modifiers = 0;
            break;    
    } *piece_no_modifiers |= COLOR_TO_MOVE_NUMBER;

    //write src and dest cells to *src and *dest
    *src = (move[1] - 'a') + (move[2] - '1') * 8;
    *dest = (move[3] - 'a') + (move[4] - '1') * 8;
    printf("%d %d %d", *piece_no_modifiers, *src, *dest);

    return 0;
}

bool is_move_valid(uint8_t piece, uint8_t src, uint8_t dest) 
{
    return true;
}









