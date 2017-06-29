#include <SDL2/SDL.h>

SDL_Renderer* renderer = NULL;
SDL_Event event;
short width = 1080;
short height = 1920;

class Buttons
{
	// que contiene
	// posciones, puntos de toque, rects de posicion fija,  metodo de activo, no activo 
	public:
	
	bool isIddle = true;
	bool isActive = false;
	bool reDraw = false;
	
	int posX = 0;
    int posY = 0;
	
	short buttonWidth = 200;
    short buttonHeight = 100;
    SDL_Rect button;
    
	
	Buttons(int posX, int posY)
	{
		 
		 button.x = posX;
		 button.y = posY;
		 button.w = buttonWidth;
		 button.h =  buttonHeight;
		 SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF ); 
		 SDL_RenderDrawRect ( renderer, &button);
	//	 SDL_RenderPresent(renderer);
		 isIddle = true;
		 
	}
	
	
	
	
	void iddleButton()
	{
		
		
		if (isIddle)
		
		
		{
                        		
                        		  if ( (  event.tfinger.x  * width>= button.x)  && (  event.tfinger.x * width< ( button.x + button.w))  )
                                   {
                        
                                          if ((event.tfinger.y * height >= button.y ) && ( event.tfinger.y * height< ( button.y + button.h))  )
                        
                        {
                        		SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );    
                        	 SDL_RenderFillRect( renderer, &button);
                   
                    isIddle = false;
                    isActive = true;
                    reDraw = false;

                        }
                                   }
                       
                    }
			
	}
	
	void activeButton()
	{
		if (isActive)
		{
			
			SDL_SetRenderDrawColor( renderer, 0x00, 0x00, 0x00, 0xFF );    
                        	 SDL_RenderFillRect( renderer, &button);
			
           SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF ); 
		 SDL_RenderDrawRect ( renderer, &button);
			
                     //   SDL_RenderPresent(renderer);
       //      SDL_SetRenderDrawColor( renderer, 0xFF, 0x00, 0x00, 0xFF );        
                
   //        SDL_RenderDrawRect( renderer, &button );
      //    SDL_RenderPresent(renderer);
           isActive = false;
           isIddle = true;
           reDraw = true;
           
		}
		
	}
		private:
};