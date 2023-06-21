#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(0);
	ofSetLineWidth(2);
	ofSetCircleResolution(30);
	ofEnableBlendMode(ofBlendMode::OF_BLENDMODE_ADD);
}

//--------------------------------------------------------------
void ofApp::update() {

	this->mesh_list.clear();

	int radius = 50;
	ofColor color;

	while (this->log_list.size() < 80) {

		int deg_base = ofRandom(360);
		int deg = deg_base / 1 * 1;

		vector<glm::vec2> log;
		auto location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
		log.push_back(location);
		auto next = glm::vec2((radius - 10) * cos(deg * DEG_TO_RAD), (radius - 10) * sin(deg * DEG_TO_RAD));

		auto distance = location - next;
		distance *= 1;

		auto future = location + distance * 30;
		auto random_deg = ofRandom(360);
		future += glm::vec2(120 * cos(random_deg * DEG_TO_RAD), 120 * sin(random_deg * DEG_TO_RAD));
		auto future_distance = future - location;

		this->log_list.push_back(log);
		this->velocity_list.push_back(glm::normalize(future_distance) * glm::length(distance) * ofRandom(0.2, 1));
		color.setHsb(ofRandom(255), 200, 255);
		this->color_list.push_back(color);
	}

	for (int i = this->log_list.size() - 1; i > -1; i--) {

		auto location = this->log_list[i][this->log_list[i].size() - 1];

		auto deg = atan2(this->velocity_list[i].y, this->velocity_list[i].x) * RAD_TO_DEG;
		deg += ofMap(ofNoise(glm::vec3(location * 0.005, ofGetFrameNum() * 0.01)), 0, 1, -20, 20);
		auto len = glm::length(this->velocity_list[i]);
		location += glm::vec2(len * cos(deg * DEG_TO_RAD), len * sin(deg * DEG_TO_RAD));

		if (glm::distance(glm::vec2(), this->log_list[i][this->log_list[i].size() - 1]) > 500) {

			this->log_list.erase(this->log_list.begin() + i);
			this->velocity_list.erase(this->velocity_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);

			continue;
		}

		this->log_list[i].push_back(location);
		this->velocity_list[i] *= 1.02;
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();

	for (int i = this->log_list.size() - 1; i > -1; i--) {

		ofSetColor(this->color_list[i], 128);
		ofFill();
		ofDrawCircle(this->log_list[i][this->log_list[i].size() - 1], 20);	

		ofSetColor(this->color_list[i], 255);
		ofNoFill();
		ofDrawCircle(this->log_list[i][this->log_list[i].size() - 1], 20);

		ofSetColor(this->color_list[i], 255);
		ofFill();
		ofDrawCircle(this->log_list[i][this->log_list[i].size() - 1], 5);

		ofNoFill();
		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}