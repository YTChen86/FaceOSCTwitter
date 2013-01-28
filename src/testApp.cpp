// A side project receiving face tracking osc messages from
// Kyle McDonald's FaceOSC https://github.com/kylemcdonald/ofxFaceTracker/downloads
// for the IACD class at the CMU School of Art
//
//
// Author: Han Hua, Jan 2013
//
// Built on a template from Dan Wilcox danomatika.com
// for the IACD class at the CMU School of Art
//
// adapted from Greg Borenstein's Processing example
// https://gist.github.com/1603230
//
// Also referenced to ofxJSON https://github.com/jefftimesten/ofxJSON
//

#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {
    // set bg to a nice grey!
	ofBackground(150);
    
    // load fonts to display stuff
	font.loadFont("futura_book.otf", 12);
	titleFont.loadFont("futura_book.otf", 20);
    
    // Enable some logging information
	ofSetLogLevel(OF_LOG_VERBOSE);
    
    //franklinBook.loadFont("frabk.ttf", 32);
	franklinBook.setLineHeight(36);
    
    string url = "http://api.twitter.com/1/trends/1.json";
    //string url = "http://api.twitter.com/1/trends/daily.json";
    //string url = "http://api.twitter.com/1.1/trends/closest.json";
    //string url = "http://api.twitter.com/1.1/search/tweets.json?q=Google";
    //string url = "http://search.twitter.com/search.json?q=%40AlanHuaCN&result_type=mixed";

    // FaceOSC sends to port 8338 by default
	receiver.setup(8338);

	ofBackground(255);
    
    // Now parse the JSON
	parsed = json.open(url);
/*	if ( parsed )
    {
		cout << json.getRawString() << endl;
		
	}
    else
    {
		cout  << "Failed to parse JSON" << endl;
	} */
    
    action_url = "http://twitter.com/statuses/update.xml";
	ofAddListener(httpUtils.newResponseEvent,this,&testApp::newResponse);
	httpUtils.start();

}

//--------------------------------------------------------------
void testApp::update() {

	// check for waiting osc messages
	while(receiver.hasWaitingMessages())
	{
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);
        
        // load face from any FaceOSC messages
        face.parseOSC(m);
	}
    
    // found face?
    if(face.found > 0) {
    //cout << "---------" << endl
      //   << face.toString();
    }
}


//--------------------------------------------------------------
void testApp::draw() {

    
    if(face.found > 0) {
        // draw a face
        
        ofPushMatrix();
            ofTranslate(face.posePosition);
            ofScale(face.poseScale, face.poseScale);
            
            ofSetColor(0);
            ofNoFill();
            ofEllipse(-20, face.eyeLeft * -9, 20, 7);
            ofEllipse(20, face.eyeRight * -9, 20, 7);
            ofEllipse(0, 20, face.mouthWidth * 3, face.mouthHeight * 3);
            ofEllipse(-5, face.nostrils * -1, 7, 3);
            ofEllipse(5, face.nostrils * -1, 7, 3);
            
            ofSetRectMode(OF_RECTMODE_CENTER);
            ofFill();
            ofRect(-20, face.eyebrowLeft * -5, 25, 5);
            ofRect(20, face.eyebrowRight * -5, 25, 5);
            ofSetRectMode(OF_RECTMODE_CORNER);
        ofPopMatrix();
        
        
        // catch emotion
        if(face.mouthHeight > 5){
            
            // Tweet
            
            if(!mouth_open) {
                tweet();
                mouth_open = true;
                
                ofDrawBitmapString(requestStr,20,20);
                ofDrawBitmapString(responseStr,20,60);
            }
            
            // fetch Twitter Trend
            if(parsed)
            {
                ofSetHexColor(0x000000);
                
                int n=0;
                ofxJSONElement trends = json[n]["trends"];
                
                for(int i=0; i<trends.size(); i++)
                {
                    string message = trends[i]["query"].asString();
                    message = ofxJSONElement::decodeURL( message );
                    //cout << message << endl;
                    font.drawString(message, 10, 40*i+40);
                    //franklinBook.drawString(message, 10, 40*i+40);
                    
                }
            }
             
        }else{
            mouth_open = false;
        }
    }

}

//--------------------------------------------------------------
void testApp::newResponse(ofxHttpResponse & response){
	responseStr = ofToString(response.status) + ": " + (string)response.responseBody;
}


void testApp::tweet() {
    
    ofxHttpForm form;
	form.action = action_url;
	form.method = OFX_HTTP_POST;
    form.addFormField("login", "faceosctwitter@hotmail.com");
	form.addFormField("password", "1q2w3e4r");
	form.addFormField("user", "FaceOSCTweet");
    form.addFormField("status", "Tweet from FaceOSCTweet, at @golan course!");
	httpUtils.addForm(form);
	
    
    /*
    CkHttp http;
    
    bool success;
    
    //  Any string unlocks the component for the 1st 30-days.
    success = http.UnlockComponent("Anything for 30-day trial");
    if (success != true) {
        printf("%s\n",http.lastErrorText());
        return;
    }
    
    //  Set the login/password for Twitter Authentication:
    http.put_Login("faceosctwitter@hotmail.com");
    http.put_Password("1q2w3e4r");
    
    CkHttpRequest req;
    req.AddParam("user","FaceOSCTweet");
    req.AddParam("status","Tweet from FaceOSCTweet, at @golan course!");
    
    CkHttpResponse *response = 0;
    response = http.PostUrlEncoded("http://twitter.com/statuses/update.xml",req);
    if (response == 0 ) {
        printf("%s\n",http.lastErrorText());
    }
    else {
        if (response->get_StatusCode() == 200) {
            printf("Tweet Successful!\n");
        }
        else {
            printf("%s\n",response->bodyStr());
        }
        
        delete response;
    }*/

}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo) { 

}
