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

#define WHITE           0x8
#define BLACK           0x10

#define CHECK           0x80

#define EN_PASSANT      0x20
#define PAWN_FIRST_MOVE 0x40

#define PIECE_TYPE_MASK 0x7


#define COLOR_TO_MOVE_LITERAL(move_count)   (move_count % 2 == 1) ? ("white") : ("black")
#define COLOR_TO_MOVE_NUMBER(move_count)    (move_count % 2 == 1) ? (WHITE) : (BLACK)


void init_board( void );
void draw_board( void );
void make_a_move(uint8_t src, uint8_t dest);

int8_t move_input(uint8_t *piece_no_modifiers, uint8_t *src, uint8_t *dest);
uint8_t piece = 0, src = 0, dest = 0;

int32_t main(int32_t argc, int8_t **argv)
{
    bool pawn_moves(uint8_t, uint8_t);
    init_board();
    draw_board();
    while(1){
    piece = 0, src = 0, dest = 0;
    move_input(&piece, &src, &dest);
    if(pawn_moves(src, dest))
        make_a_move(src, dest);
    draw_board();
    
    }
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
    move_count++;
}

// move input, expects piece+src_square+dest_square
// for example, pd2d4 qa4d1 
int8_t move_input(uint8_t *piece_no_modifiers, uint8_t *src, uint8_t *dest) 
{
    // clean pointers values 
    *piece_no_modifiers = 0, *src = 0, *dest = 0;

    // input itself
    printf("Make a move (%s to move): ", COLOR_TO_MOVE_LITERAL(move_count));
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
    } *piece_no_modifiers |= COLOR_TO_MOVE_NUMBER(move_count);

    //write src and dest cells to *src and *dest
    *src = (move[1] - 'a') + (move[2] - '1') * 8;
    *dest = (move[3] - 'a') + (move[4] - '1') * 8;
    printf("%d %d %d", *piece_no_modifiers, *src, *dest);

    return 0;
}

bool is_move_valid(uint8_t piece, uint8_t src, uint8_t dest) 
{
    // check if destination square is behind the source square    
    if(dest <= src)
        return false;
}

bool is_checked() 
{

}

bool pawn_moves(uint8_t src, uint8_t dest)
{
    uint8_t offset = dest - src;
    uint8_t dest_square = chess_board[dest / 8][dest % 8];

    // check if it's a left-side capture
    if(offset == 7) {
        // check if it's on the left-most column
        if(src % 8 == 0)
            return false;
        // check if it's same colored pieces
        if(dest_square & COLOR_TO_MOVE_NUMBER(move_count))
            return false;
        // check if the square is empty --REDUNDANT--
        if(dest_square == 0)
            return false;
        return true;
    }

    // check if it's a 1-square-up move
    else if(offset == 8) {
        // check if the square is empty
        if(dest_square != 0)
            return false;
        return true;
    }

    // check if it's a right-side capture
    else if(offset == 9) {
        // check if it's on the right-most column
        if(src % 8 == 7)
            return false;
        // check if it's same colored pieces
        if(dest_square & COLOR_TO_MOVE_NUMBER(move_count))
            return false;
        // check if the square is empty --REDUNDANT--
        if(dest_square == 0)
            return false;
        return true;  
    }

    // check if it's a 2-square-up move
    else if(offset == 16) {
        // check if the jumped-over and dest squares are empty
        if(dest_square != 0 && dest_square - 8 != 0)
            return false;
        return true;
    }
    return false;
}

bool rook_moves(uint8_t src, uint8_t dest) 
{
    uint8_t offset = dest - src;
    uint8_t dest_square = chess_board[dest / 8][dest % 8];
    
    // check if rook moves horizontally
    if(offset < 8) {
        // check if the dest[][this] bigger than src[][this]  
        if(dest % 8 <= src % 8)
            return false;
        // check every square whether it's empty
        for(uint8_t i = (src % 8) + 1; i < dest % 8; i++) {
            if(chess_board[src / 8][i] != 0)
                return false;
        }
        // check the destination square
            if(dest_square & COLOR_TO_MOVE_NUMBER(move_count) != 0)
                return false;
        return true;
    }

    // check if rook moves vertically
    else if(dest % 8 == src % 8 && offset >= 8) {
        // check every square whether it's empty
        for(uint8_t i = src; i < dest; i += 8) {
            if(chess_board[i / 8][i % 8] != 0)
                return false;
        }
        // check the destination square
            if(dest_square & COLOR_TO_MOVE_NUMBER(move_count) != 0)
                return false;
        return true;
    }
}

bool knight_moves(uint8_t src, uint8_t dest)
{

}

bool bishop_moves(uint8_t src, uint8_t dest) 
{

}

bool queen_moves(uint8_t src, uint8_t dest) 
{

}

bool king_moves(uint8_t src, uint8_t dest)
{

}