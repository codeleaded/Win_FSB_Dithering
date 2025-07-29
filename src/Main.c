#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Library/ImageFilter.h"


int QuantiseN;
Sprite original;
Sprite quantised;
Sprite dithered;
Sprite blurred;

Sprite* selected;
TransformedView tv;


Pixel QuantiseN_G_Func(Pixel p){
    return Pixel_GQ_N(p,QuantiseN);
}
Pixel QuantiseN_C_Func(Pixel p){
    return Pixel_CQ_N(p,QuantiseN);
}
Pixel QuantiseN_CC_Func(Pixel p){
    return Pixel_CCQ_N(p,(Pixel[]){
        BLACK,WHITE,RED,GREEN,BLUE
    },5);
}

void Setup(AlxWindow* w){
    QuantiseN = 1;
    
    tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
    original = Sprite_Load("./data/Dogs_Snow.jpg");
    quantised = Sprite_MakeForAll(&original,QuantiseN_CC_Func);
    dithered = ImageFilter_FSB_Dithering(&original,QuantiseN_CC_Func);
    //blurred = ImageFilter_C_LI(&dithered);
    blurred = ImageFilter_C_CI(&dithered);

    selected = &original;
}

void Update(AlxWindow* w){
    TransformedView_HandlePanZoom(&tv,window.Strokes,GetMouse());

    if(Stroke(ALX_KEY_W).PRESSED) QuantiseN++;
    if(Stroke(ALX_KEY_S).PRESSED) QuantiseN--;

    if(Stroke(ALX_KEY_2).PRESSED) selected = &original;
    if(Stroke(ALX_KEY_3).PRESSED) selected = &quantised;
    if(Stroke(ALX_KEY_4).PRESSED) selected = &dithered;
    if(Stroke(ALX_KEY_5).PRESSED) selected = &blurred;

    if(Stroke(ALX_KEY_1).PRESSED){
        Sprite_ForAll(&original,&quantised,QuantiseN_CC_Func);

        Sprite_Free(&dithered);
        dithered = ImageFilter_FSB_Dithering(&original,QuantiseN_CC_Func);
        blurred = ImageFilter_C_CI(&dithered);
    }
    
    Clear(BLACK);

    //Sprite_Render(WINDOW_STD_ARGS,&sp,0.0f,0.0f);

    for(int i = 0;i<GetHeight();i++){
        for(int j = 0;j<GetWidth();j++){
            Vec2 p = TransformedView_ScreenWorldPos(&tv,(Vec2){ j,i });
            if(p.x>=0.0f && p.x<1.0f && p.y>=0.0f && p.y<1.0f) window.Buffer[i * GetWidth() + j] = Sprite_Sample(selected,p.x,p.y);
        }
    }

    String str = String_Format("QuantiseN: %d",QuantiseN);
    CStr_RenderSizeAlxFont(WINDOW_STD_ARGS,&window.AlxFont,str.Memory,str.size,0.0f,0.0f,RED);
    String_Free(&str);
}

void Delete(AlxWindow* w){
    Sprite_Free(&original);
    Sprite_Free(&quantised);
    Sprite_Free(&dithered);
    Sprite_Free(&blurred);

    selected = NULL;
}

int main(){
    if(Create("FSB Dithering",1900,1000,1,1,Setup,Update,Delete))
        Start();
    return 0;
}