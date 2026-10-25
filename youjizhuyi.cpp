#include<iostream>
#include<vector>
#include<string>
#include<opencv2/opencv.hpp>
using namespace std;
#define ts_1 1000
#define ts_2 50
#define MAX_LINES 6
#define LINE_HEIGHT 30
#define FONT_SIZE 1
#define FONT_THICKNESS 1
int a[10010];
vector<string> splitUTF8(const string &s){
    vector<string> chars;
    for(size_t i=0;i<s.size();){
        unsigned char c=s[i];
        int len;
        if(c<0x80)len=1;
        else if(c<0xE0)len=2;
        else if(c<0xF0)len=3;
        else len=4;
        chars.push_back(s.substr(i,len));
        i+=len;
    }
    return chars;
}
static int centeredX(const cv::Mat &canvas, const string &text){
    int baseline=0;
    cv::Size sz = cv::getTextSize(text,cv::FONT_HERSHEY_SIMPLEX,FONT_SIZE,FONT_THICKNESS,&baseline);
    return std::max(0,(canvas.cols-sz.width)/2);
}
cv::Mat renderUI(cv::Mat &srcImg,const vector<string>& lines,const string& prompt=""){
    int textAreaH=MAX_LINES*LINE_HEIGHT+70;
    cv::Mat out(cv::Size(srcImg.cols,srcImg.rows+textAreaH),srcImg.type(),cv::Scalar(0,0,0));
    srcImg.copyTo(out(cv::Rect(0,0,srcImg.cols,srcImg.rows)));
    int y=srcImg.rows+55;
    for(auto &line:lines){
        cv::putText(out,line,cv::Point(centeredX(out,line),y),cv::FONT_HERSHEY_SIMPLEX,FONT_SIZE,cv::Scalar(255,255,255),FONT_THICKNESS);
        y+=LINE_HEIGHT;
    }
    if(!prompt.empty()){
        cv::putText(out,prompt,cv::Point(centeredX(out,prompt),y),cv::FONT_HERSHEY_SIMPLEX,FONT_SIZE,cv::Scalar(0,165,255),FONT_THICKNESS);
    }
    
    return out;
}
void t_print(cv::Mat &baseImg,vector<string>& buf,const string &s,bool waitEnd=true){
    vector<string> chars=splitUTF8(s);
    string curLine;
    for(auto &ch:chars){
        curLine+=ch;
        if(buf.empty())
            buf.push_back(curLine);
        else 
            buf.back()=curLine;
        cv::Mat show=renderUI(baseImg,buf,"");
        cv::imshow("故事窗口",show);
        cv::waitKey(ts_2);
    }
    if(waitEnd){
        buf.push_back("");
        cv::Mat show=renderUI(baseImg,buf,"");
        cv::imshow("故事窗口",show);
        cv::waitKey(ts_1);
    }
    else{
        buf.push_back("");
        cv::Mat show=renderUI(baseImg,buf,"请输入选择：按 1 / 2");
        cv::imshow("故事窗口",show);
    }
}
int waitSelect(cv::Mat &baseImg,vector<string>& buf){
    while(true){
        int key=cv::waitKey(20);
        if(key=='1')return 1;
        if(key=='2')return 2;
        cv::Mat show=renderUI(baseImg,buf,"请输入选择：按 1 / 2");
        cv::imshow("故事窗口",show);
    }
}

void clearScreen(vector<string>& buf){
    buf.clear();
}
int main(){
    cv::startWindowThread();
    cv::namedWindow("故事窗口",cv::WINDOW_NORMAL);
    cv::Mat img=cv::imread("input.png");
    vector<string> textBuf;
    t_print(img,textBuf,"14班成立，班内同学关系不佳。");
    t_print(img,textBuf,"高皓远认为从竞赛班淘汰的樊叙“装”。");
    t_print(img,textBuf,"樊叙看不惯唐颖怼老师，发生口角。");
    clearScreen(textBuf);
    t_print(img,textBuf,"小马在12班讲课被学生刁难。");
    t_print(img,textBuf,"校方不允许竞赛班以外的学生参加化学竞赛。");
    t_print(img,textBuf,"1:14班凭“化学杀”实力说服了化学老师陈阳，最终得到她的支持。");
    t_print(img,textBuf,"2:14班没能成功说服化学老师陈阳。",false);
    a[0]=waitSelect(img,textBuf);
    clearScreen(textBuf);
    switch(a[0]){
        case 1:
            t_print(img,textBuf,"十一假期樊叙参加化学竞赛，周翼离家出走。");
            t_print(img,textBuf,"周翼被找回，接受批评教育。");
            t_print(img,textBuf,"1:进一步指责。");
            t_print(img,textBuf,"2:深入了解具体情况。",false);
            a[1]=waitSelect(img,textBuf);
            clearScreen(textBuf);
            switch(a[1]){
                case 1:
                    break;
                case 2:
                    break;
            }
            t_print(img,textBuf,"化学竞赛出结果，樊叙、游智慧落榜，姜博入围国赛。");
            t_print(img,textBuf,"实习老师给12班代生物课，有学生拒不配合，认为年轻老师不行。");
            t_print(img,textBuf,"14班为老师出头。");
            t_print(img,textBuf,"1:不作为。");
            t_print(img,textBuf,"2:劝告主谋高皓远下次不要这样。",false);
            a[2]=waitSelect(img,textBuf);
            clearScreen(textBuf);
            switch(a[2]){
                case 1:
                    break;
                case 2:
                    break;
            }
            break;
    }
    clearScreen(textBuf);
    t_print(img,textBuf,"14班初次月考成绩不尽人意，小马遭到压力，立下flag要让班级平均达到550。");
    t_print(img,textBuf,"游智慧指出“学历并非唯一标准”。");
    t_print(img,textBuf,"1:觉得她说的对，但是告诉大家“高考也是人生很重要的一步”。");
    t_print(img,textBuf,"2:否认她的观点，使她愤然离去。",false);
    a[3]=waitSelect(img,textBuf);
    clearScreen(textBuf);
    switch(a[3]){
        case 1:
            t_print(img,textBuf,"樊叙成为数学课代表，与之前关系一般的朱李泰共事，解开了对于数学的心理压力。");
            break;
        case 2:
            t_print(img,textBuf,"我后来才明白，人的离开是不需要理由的。");
            t_print(img,textBuf,"就像那盆绿萝，我每天浇水，它还是枯了。我把它放在窗台上，想着哪天再买一盆新的，后来就忘了。");
            t_print(img,textBuf,"再后来座位空了，我最后一次回头看了一眼14班的窗户，窗帘拉着，里面什么都没有。");
            t_print(img,textBuf,"我原本以为自己可以改变什么的。");
            clearScreen(textBuf);
            t_print(img,textBuf,"刚来的时候，我写过一张纸条贴在办公桌边上：“没有坏学生，只有还没被看见的学生。");
            t_print(img,textBuf,"那张纸条贴了很久，边角卷了，字迹也模糊了。");
            t_print(img,textBuf,"走的那天我把它撕下来，折了四折，放进大衣口袋里。");
            t_print(img,textBuf,"后来洗衣服的时候忘了掏出来，它就碎了。");
            clearScreen(textBuf);
            t_print(img,textBuf,"我走的那天是个晴天。");
            t_print(img,textBuf,"阳光特别亮，照着走廊，照着空无一人的教室，照着桌上那盆枯萎的绿萝。");
            t_print(img,textBuf,"我没有回头，也没有哭。我只是一直往前走，走到校门口的时候停了一下，然后继续走。");
            clearScreen(textBuf);
            t_print(img,textBuf,"后来有人问我，后悔吗。");
            t_print(img,textBuf,"我说不后悔。我不是不后悔，我是不能说后悔——如果说后悔，那这一年就真的什么都没剩下。");
            clearScreen(textBuf);
            t_print(img,textBuf,"你问我那一年留下了什么。");
            t_print(img,textBuf,"我留下的不是分数，不是排名，不是高考成绩单上的数字。");
            t_print(img,textBuf,"我留下的是那天阳光落在你脸上的样子,");
            t_print(img,textBuf,"是你第一次叫我“老师”的时候声音还有点抖，");
            t_print(img,textBuf,"是你后来不再躲我的眼神。");
            t_print(img,textBuf,"这些就够了。");
            clearScreen(textBuf);
            t_print(img,textBuf,"我走了之后，有人接了我的班，继续坐在那间办公室里。");
            t_print(img,textBuf,"绿萝被扔掉了，新老师买了新的盆栽放在窗台上，比我的那盆绿。");
            clearScreen(textBuf);
            t_print(img,textBuf,"14班的门牌被换掉了。新的学生走进去，新的老师在黑板上写字，新的阳光照进来，新的灰尘落下去。");
            t_print(img,textBuf,"一切都和我来之前一样。");
            t_print(img,textBuf,"一切都和我走之后一样。");
            t_print(img,textBuf,"时间落下来，变成一层很薄很薄的灰。我走过去，留下脚印，然后脚印也被覆盖了。");
            t_print(img,textBuf,"但我记得。我记得你们。");
            clearScreen(textBuf);
            t_print(img,textBuf,"结局1：和光同尘");
            cv::waitKey(0);
            cv::destroyAllWindows();
            return 0;
    }
    clearScreen(textBuf);
    t_print(img,textBuf,"姜博为完成社团任务拍摄了微电影，并上传视频软件，主题是“别让过度内卷磨灭了自己的个性。");
    t_print(img,textBuf,"被教导主任看到，举报给小马和校长。作为导演和主演的姜博受处分风险，14班一齐为他求情。");
    t_print(img,textBuf,"恰逢教育局突击检查素质教育落实情况，校长顺势将事件包装成“学生创新实践案例”，事件最终平息。");
    t_print(img,textBuf,"姜博受到一定打击");
    clearScreen(textBuf);
    t_print(img,textBuf,"樊叙第二次月考后回12班和朋友交流学习，遭到旧时相伴覃正贬低。");
    t_print(img,textBuf,"于是周翼为了给樊叙出路跟他打了一架，最后周翼樊叙覃正都被请了家长。");
    t_print(img,textBuf,"1:叙翼因不信任而分手。");
    t_print(img,textBuf,"2:叙翼关系因为此次插曲更加紧密。",false);
    a[4]=waitSelect(img,textBuf);
    clearScreen(textBuf);
    switch(a[4]){
        case 1:
            t_print(img,textBuf,"结局2-光谱红移");
            cv::waitKey(0);
            cv::destroyAllWindows();
            return 0;
        case 2:
            break;
    }
    t_print(img,textBuf,"小马和12班班主任赵姗算是结下梁子了，赵姗还在办公室贬低过樊叙，被小马反驳。");
    clearScreen(textBuf);
    t_print(img,textBuf,"小马过年期间回家，被催婚，且家里人都不太看好她的工作。");
    t_print(img,textBuf,"辩论大会，题目“优绩主义究竟是社会进步的动力，还是加剧不公与焦虑的枷锁”。");
    t_print(img,textBuf,"高皓远带领 14 班同学反抗强权压制。");
    clearScreen(textBuf);
    t_print(img,textBuf,"临近模考，赵姗把小马因学术纠纷肄业的事情抖落出来，小马被12班家长联名抵制。");
    t_print(img,textBuf,"但是由于14班同学的坚持，她继续担任14班班主任");
    cv::waitKey(0);
    cv::destroyAllWindows();
    return 0;
}
