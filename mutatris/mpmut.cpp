#include "mpmut.h"
#include<cstdlib>



namespace mp
    {
    
    blockType::blockType(const blockType &t)
    {
        operator=(t);
    }
    
    void blockType::operator=(const blockType &t)
    {
        x = t.x;
        y = t.y;
        z = t.z;
        index_color = t.index_color;
    }
    
    bool blockType::operator==(const blockType &t)
    {
        
        if(t.x == x && t.y == y && t.z == z && t.index_color == index_color) return true;
        return false;
    }
    
    
    
    gameBlock::gameBlock(const gameBlock &b)
    {
        operator=(b);
    }
    
    void gameBlock::operator=(const gameBlock &b)
    {
        x = b.x;
        y = b.y;
        z = b.z;
        blocks[0] = b.blocks[0];
        blocks[1] = b.blocks[1];
        blocks[2] = b.blocks[2];
        blocks[3] = b.blocks[3];
    }
    
    
    void gameBlock::randBlock()
    {
        
        y = 0;
        x = (25)/2;
        z = 0;
        do
        {
            blocks[0].index_color = 1+rand()%5;
            blocks[1].index_color = 1+rand()%5;
            blocks[2].index_color = 1+rand()%5;
            blocks[3].index_color = 1+rand()%5;
            
        } while (blocks[0].index_color == blocks[1].index_color && blocks[0].index_color == blocks[2].index_color && blocks[0].index_color == blocks[3].index_color );
        
        
        block_type = static_cast<block_t>(rand()%3);
        
        
        blocks[0].z = blocks[1].z = blocks[2].z = blocks[3].z = 0;
        
        switch(block_type)
        {
                /* 1 */case BLOCK_HORIZ:
                blocks[0].x = 0;
                blocks[0].y = 0;
                blocks[0].z = 0;
                blocks[1].x = 0;
                blocks[1].y = 1;
                blocks[1].z = 0;
                blocks[2].x = 0;
                blocks[2].z = 0;
                blocks[2].y = 2;
                blocks[3].x = 0;
                blocks[3].y = 3;
                blocks[3].z = 0;
                break;
                /* 1 */case BLOCK_VERT:
                blocks[0].x =  0;
                blocks[0].y = 0;
                blocks[0].z = 0;
                blocks[1].x = 1;
                blocks[1].y = 0;
                blocks[3].z = 0;
                blocks[2].x = 2;
                blocks[2].y = 0;
                blocks[2].z = 0;
                blocks[3].x = 3;
                blocks[3].y = 0;
                blocks[3].z = 0;
                break;
                /* 1 */case BLOCK_SQUARE:
                blocks[0].x = 0;
                blocks[0].y = 0;
                blocks[0].z = 0;
                blocks[1].z = 0;
                blocks[2].z = 0;
                blocks[3].z = 0;
                blocks[1].x = 0;
                blocks[1].y = 1;
                blocks[3].x = 1;
                blocks[3].y = 0;
                blocks[2].x = 1;
                blocks[2].y = 1;
                break;
        }
        
        
    }
    
    mxMut::mxMut(): nscore(0), nclear(0), grid(this)
    {
        
        grid.score = 0;
        grid.blocks_cleared = 0;
        freez = false;
        
    }
    
    void mxMut::newGame()
    {
        
        grid.clear();
        grid.score =  0;
        grid.blocks_cleared = 0;
        next.randBlock();
        nextBlock();
        //setTimer(600);
        game_over = false;
        inc_s = inc_c = 0;
        increase = false;
        
    }
    
    void mxMut::moveOutward() {
        
        int i = 0;
        bool go = true;
        for(i=0; i < 4; i++) {
            int new_z = current.z-current.blocks[i].z;
            new_z = !new_z;
            if(grid.data[current.x+current.blocks[i].x][current.y+current.blocks[i].y][new_z] != 0) {
                go = false;
                break;
            }
        }
        
        if(go == true) {
            if(current.z > 0) --current.z;
        }
    }
    
    
    void mxMut::moveInward() {
        int i = 0;
        bool go = true;
        for(i=0; i < 4; i++) {
            int new_z = current.z+current.blocks[i].z;
            new_z = !new_z;
            if(grid.data[current.x+current.blocks[i].x][current.y+current.blocks[i].y][new_z] != 0) {
                go = false;
                break;
            }
        }
        
        if(go == true) {
            if(current.z < GRID_Z-1) ++current.z;
        }
    }
    
    
    void mxMut::moveLeft()
    {
        
        int i;
        bool go = true;
        
        // scan for collisions
        
        for(i=0;i<4;i++)
        {
            
            if(current.x+current.blocks[i].x > 0)
            {} else { go = false; break; }
            
            if(grid.data[current.x+current.blocks[i].x-1][current.y+current.blocks[i].y][current.z+current.blocks[i].z] != 0)
            { go = false; break; }
            
            
        }
        
        if(go == true)
            current.x--;
        
    }
    
    void mxMut::moveRight()
    {
        
        int i;
        bool go = true;
        
        // scan for collisoins
        
        for(i=0;i<4;i++)
        {
            if(current.x+current.blocks[i].x<(grid.grid_w-1))
            {} else { go = false; break; }
            
            if(grid.data[current.x+current.blocks[i].x+1][current.y+current.blocks[i].y][current.z+current.blocks[i].z] != 0)
            { 	go = false; break; }
            
            
        }
        
        
        if(go == true)
            current.x++;
        
    }
    
    void mxMut::moveDown()
    {
        update_moveDown();
    }
    
    void mxMut::shiftColor()
    {
        // shift color
        int colors[4];
        
        for(int i = 0; i < 4; i++)
            colors[i] = current.blocks[i].index_color;
        
        current.blocks[0].index_color = colors[3];
        current.blocks[1].index_color = colors[0];
        current.blocks[2].index_color = colors[1];
        current.blocks[3].index_color = colors[2];
        
    }
    
    void mxMut::nextBlock()
    {
        current = next;
        next.randBlock();
    }
    
    void mxMut::update_moveDown()
    {
        
        int i;
        bool go = true;
        
        for(i = 0; i < 4; i++)
        {
            
            if(current.y+current.blocks[i].y > grid.grid_h-2)
            {
                update_mergeBlock();
                return;
            } else go = true;
            
            
            if(grid.data[current.x+current.blocks[i].x][current.y+current.blocks[i].y+1][current.z+current.blocks[i].z] != 0)
            {
                if(current.y > 3) update_mergeBlock();
                else {
                    //stopTimer();
                    game_over = true;
                    
                }
                return;
            }
            
        }
        
        if(go == true)
            current.y++;
        
    }
    
    
    void mxMut::update_mergeBlock()
    {
        
        grid.merge(current);
        nextBlock();
    }
    
    void mxMut::update() {
        /*	unsigned int mxMut::timerExecution(unsigned int mil)
         { */
        //if(freez == true) return 0;
        
        update_moveDown();
        
        //		grid.update();
        
        /*	return mil;
         }*/
    }
    
    
    }
    
    
