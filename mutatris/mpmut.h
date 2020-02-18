#ifndef __MX_MP_MUT_
#define __MX_MP_MUT_
#include<cstdlib>
#include<cstring>
namespace mp
{

	class blockType {
	public:

		blockType() { x = 0, y = 0, z = 0; index_color = 0;}
		blockType(int x, int y, int z, unsigned int index_color) { this->x = x, this->y = y, this->index_color = index_color, this->z = z;  }
		blockType(const blockType &t);

		void operator=(const blockType &t);
		bool operator==(const blockType &t);

		int x,y,z;
		int index_color;
	};


	class gameBlock {
	public:
		enum block_t { BLOCK_VERT, BLOCK_HORIZ, BLOCK_SQUARE };
		int x,y,z;
		blockType blocks[4];
		block_t block_type;
		gameBlock() { x = 0; y = 0; z = 0; }
		gameBlock(const gameBlock &c);
		void operator=(const gameBlock &b);
		void randBlock();
	};

	class mxMut;


	template<size_t width, size_t height, size_t depth>
	class mpGrid {

	public:
		enum { DATA_WIDTH=width,DATA_HEIGHT=height,DATA_DEPTH=depth };
		int data[width][height][depth];
		int grid_w,grid_h,grid_z;
		mpGrid(mxMut *m) { this->mut = m; grid_w =width; grid_h=height; memset(data, 0, sizeof(data)); }
		void addScore(int, int);
		void clear();
		void merge(const gameBlock &block);
		void update();
		void merge_down();
		int score, blocks_cleared;
		mxMut *mut;

	};


	class mxMut  {

	public:


		mxMut();

		void newGame();
		void moveLeft();
		void moveRight();
		void moveDown();
		void moveInward();
        void moveOutward();
		void shiftColor();
		void nextBlock();
		void update();

		bool is_gameOver() const { return game_over; }

		int nscore, nclear;
		bool increase;

		enum { GRID_W = 25,
		       GRID_H = 21, GRID_Z=4};

		mpGrid<GRID_W, GRID_H, GRID_Z> grid;
		gameBlock current, next;
		bool game_over;
		int inc_s, inc_c;
		void addn(int ns, int nc) {

			nscore = ns, nclear  = nc; inc_s += ns, inc_c += nc;
			if((inc_c % 20) == 0) {
				increase = true;
			}

		}
		void freeze(bool t) { freez = t; }

		bool freez;
		void update_moveDown();

protected:
		void update_mergeBlock();


	};




	template<size_t width, size_t height, size_t depth>
	void mpGrid<width,height,depth>::addScore(int score, int cleared)
	{
		this->score += score;
		this->blocks_cleared += cleared;
		mut->addn(score,cleared);

	}



	template<size_t width, size_t height, size_t depth>
	void mpGrid<width,height,depth>::clear()
	{

		memset(data, 0, sizeof(data));


	}

	template<size_t width, size_t height, size_t depth>
	void mpGrid<width,height,depth>::merge(const gameBlock &block)
	{
		int offset_x = block.x;
		int offset_y = block.y;
		int offset_z = block.z;
		for(unsigned int i = 0; i < 4; i++)
		{
			int cx = block.blocks[i].x;
			int cy = block.blocks[i].y;
			int cz = block.blocks[i].z;
			cx+=offset_x;
			cy+=offset_y;
			cz+=offset_z;
			if(cx >= 0 && cx < static_cast<int>(width) && cy >= 0 && cy < static_cast<int>(height))
			{
				data[cx][cy][cz] = block.blocks[i].index_color;
			}
		}

	}


	template<size_t width, size_t height, size_t depth>
	void mpGrid<width,height,depth>::merge_down()
	{
		size_t i = 0, z = 0, x_depth = 0;

		for(i=0; i<width;i++)
		{
			for(z = 0; z < height-1; z++)
			{
				for(x_depth=0;x_depth<depth;++x_depth) {
					if(data[i][z][x_depth] != 0 && data[i][z+1][x_depth] == 0)
					{
						data[i][z+1][x_depth] = data[i][z][x_depth];
						data[i][z][x_depth] = 0;
						break;
					}
				}
			}
		}
	}


	template<size_t width, size_t height, size_t depth>
	void mpGrid<width,height,depth>::update()
	{
		size_t i,z,d;
		// left to right 4 in  a row vertical
		for(i = 0; i < width-3; i++)
		{
			for(z = 0; z < height; z++)
			{
				for(d = 0; d < depth; d++) {
					int cur_item = data[i][z][d];
					if(cur_item == 0 || cur_item == -1) continue;
					if(cur_item == data[i+1][z][d] && cur_item == data[i+2][z][d] && cur_item == data[i+3][z][d])
					{
						data[i][z][d] = -1;
						data[i+1][z][d] = -1;
						data[i+2][z][d] = -1;
						data[i+3][z][d]= -1;
						addScore(2,4);
						continue;
					}
				}
			}

		}
		// horizontal lines
		for(i = 0; i < width; i++)
		{
			for(z = 0; z < height-3; z++)
			{
				for(int d = 0; d < depth; d++) {
					int cur_item = data[i][z][d];
					if(cur_item == 0 || cur_item == -1) continue;
					if(cur_item == data[i][z+1][d] && cur_item == data[i][z+2][d] && cur_item == data[i][z+3][d])
					{
						data[i][z][d] = -1;
						data[i][z+1][d] = -1;
						data[i][z+2][d] = -1;
						data[i][z+3][d] = -1;
						addScore(3,4);
					}
				}
			}
		}
		//  removes squares
		for(i = 0; i < width-1; i++)
		{
			for(z = 0; z < height-1; z++)
			{
				for(int d = 0; d < depth; d++) {
					int cur_item = data[i][z][d];
					if(cur_item == 0 || cur_item == -1) continue;
					if(cur_item == data[i][z+1][d] && cur_item == data[i+1][z][d] && cur_item == data[i+1][z+1][d])
					{
						data[i][z][d] = -1;
						data[i+1][z][d] = -1;
						data[i+1][z+1][d] = -1;
						data[i][z+1][d] = -1;
						addScore(10,4);
					}
				}
			}
		}
		merge_down();
	}



}


#endif




