#include "realga.h"
#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <algorithm>
#include <sstream>
#include <cstdio>      /* printf, NULL */
#include <cstdlib>
#include <chrono>
#include<sstream>

using namespace std;




// ================================================= Java Engine =========================
#define DEGREES_TO_RADIANS 0.017453292519943295
#define RADIANS_TO_DEGREES 57.29577951308232
#define M_PI2 6.283185307179586

const double EPSILON = 0.00001;
const int Laps = 1;
const int Width = 16000;
const int Height = 9000;
const double CheckpointRadius = 600;
const double MAX_ROTATION_PER_TURN = M_PI / 10;
const int CAR_MAX_THRUST = 200;
const double CAR_FRICTION = 0.15;

bool DEBUG_COLLISION=false;


struct Utility {
    static int round(double x) {
        int s = x < 0 ? -1 : 1;
        return s * (int)std::round(s * x);
    }

    static int truncate(double x){
        //Optimization to fix rounding errors. No need to replicate
        double roundedX = Utility::round(x);
        if(fabs(roundedX-x) < EPSILON){
            return (int)roundedX;
        }

        return (int)(x < 0 ? ceil(x) : floor(x));
    }

    static double roundToTwoDecimals(double a){
        return std::round(a * 100.0) / 100.0;
    }
};

struct Point {
    double x;
    double y;

    Point() {
        x = 0.0;
        y = 0.0;
    }

    Point(double xx, double yy) {
        x = xx;
        y = yy;
    }

    double distance(const Point &p) {
        double dx = x-p.x;
        double dy = y-p.y;
        return sqrt(dx*dx + dy*dy);
    }

    double getAngle(const Point &p2){
        double dx = p2.x-x;
        double dy = p2.y-y;
        return atan2(dy, dx);
    }
    

    // Move the point to x and y
    void move(double xx, double yy) {
        x = xx;
        y = yy;
    }

    // Move the point to an other point for a given distance
    void moveTo(const Point &p, double dist) {
        double d = distance(p);

        if (d < EPSILON) {
            return;
        }

        double dx = p.x - x;
        double dy = p.y - y;
        double coef = dist / d;

        x += dx * coef;
        y += dy * coef;
    }

    Point getPoint(Point target, double dist){
        double d = distance(target);

        if (d < EPSILON) {
            return target.clonePoint();
        }

        double dx = target.x - x;
        double dy = target.y - y;
        double coef = dist / d;

        double new_x = x + dx * coef;
        double new_y = y + dy * coef;
        return Point(new_x, new_y);
    }

    bool isInRange(Point &p, double range) {
        return p.x != x && p.y != y && distance(p) <= range;
    }

    Point clonePoint(){
        return Point(x, y);
    }
};


struct Unit: public Point {
    double vx;
    double vy;
    double mass = 1.0;
    double friction = 0.15;

    Unit() : Point() {
        vx = 0;
        vy = 0;
    }

    Unit(double xx, double yy) : Point(xx, yy) {
        vx = 0.0;
        vy = 0.0;
    }

    double getSpeed(){
        return sqrt(vx*vx+vy*vy);
    }

    double getSpeedAngle(){
        return atan2(vy, vx);
    }

    void move(double t) {
        x += vx * t;
        y += vy * t;
    }

    void adjustPosVel() {
        x = Utility::truncate(x);
        y = Utility::truncate(y);

        vx = Utility::truncate(vx * (1.0 - friction));
        vy = Utility::truncate(vy * (1.0 - friction));
    }

    double getCollision(const Unit &u, double checkedRadius) {
        // Check instant collision
        if(DEBUG_COLLISION) {
            cerr << "DEBUG getCollision: pos" << x << " " << y << endl;
            cerr << "DEBUG getCollision: distance " << distance(u) << endl;
        }
        if (distance(u) <= checkedRadius) {
            return 0.0;
        }

        // Both units are motionless
        if (vx == 0.0 && vy == 0.0 && u.vx == 0.0 && u.vy == 0.0) {
            return 2.0;
        }

        // Change referencial
        // Unit u is not at point (0, 0) with a speed vector of (0, 0)
        double x2 = x - u.x;
        double y2 = y - u.y;
        double r2 = checkedRadius;
        double vx2 = vx - u.vx;
        double vy2 = vy - u.vy;
        
        //vx2 = Utility::truncate(vx * (1.0 - friction));
        //vy2 = Utility::truncate(vy * (1.0 - friction));

        double a = vx2 * vx2 + vy2 * vy2;

        if (a <= 0.0) {
            return 2.0;
        }

        double b = 2.0 * (x2 * vx2 + y2 * vy2);
        double c = x2 * x2 + y2 * y2 - r2 * r2;
        double delta = b * b - 4.0 * a * c;

        if (delta < 0.0) {
            return 2.0;
        }

        double t = (-b - sqrt(delta)) / (2.0 * a);

        if (t <= 0.0) {
            return 2.0;
        }

        return t;
    }
};


struct Car: public Unit {
    double angle;
    double prevAngle;
    bool debug;
    int thrust = 0;
    Point target;
    
    Car() {
        this->angle = 0.0;
        this->friction = CAR_FRICTION;  
    }

    
    Car(double xx, double yy, double a) : Unit(xx, yy) {
        this->angle = a;
        this->friction = CAR_FRICTION;
    }


    void handleInput(int c_x, int c_y, int thrust) {
        this->prevAngle = angle;

        this->target = Point(c_x, c_y);

        if(thrust < 0 || thrust > CAR_MAX_THRUST) {
            cerr << "Invalid thrust. Please keep between 0 and " << CAR_MAX_THRUST << endl;
            return;
        }

        if (this->x != c_x || this->y != c_y) {
            double targetAngle = getAngle(Point(c_x, c_y));
            double relativeAngle = shortAngleDist(this->angle, targetAngle);
            if (fabs(relativeAngle) >= MAX_ROTATION_PER_TURN) {
                int sign = relativeAngle >= 0 ? 1 : -1;
                targetAngle = this->angle + MAX_ROTATION_PER_TURN * sign;
            }

            this->angle = targetAngle;
            thrustTowardsHeading(thrust);
        }
    }


    void thrustTowardsHeading(int thrust){
        this->vx += cos(angle) * thrust;
        this->vy += sin(angle) * thrust;
    }

    void adjust() {
        adjustPosVel();
        double degrees = round(angle*RADIANS_TO_DEGREES);
        this->angle = degrees * DEGREES_TO_RADIANS;
        while(this->angle > M_PI2) this->angle-= M_PI2;
        while(this->angle < 0) this->angle+= M_PI2;
    }

    double shortAngleDist(double a0, double a1) {
        double mm = M_PI2;
        double da = fmod((a1 - a0), mm);
        return fmod(2 * da , mm) - da;
    }


	float getDiffAngle(float custom_angle, float a) {
		// To know whether we should turn clockwise or not we look at the two ways and keep the smallest
		// The ternary operators replace the use of a modulo operator which would be slower
		float right = custom_angle <= a ? a - custom_angle : M_PI2 - custom_angle + a;
		float left = custom_angle >= a ? custom_angle - a : custom_angle + M_PI2 - a;

		if (right < left) {
			return right;
		} else {
			// We return a negative angle if we must rotate to left
			return -left;
		}
	}

	float getDiffAngle(float a) {
		// To know whether we should turn clockwise or not we look at the two ways and keep the smallest
		// The ternary operators replace the use of a modulo operator which would be slower
		float right = this->angle <= a ? a - this->angle : M_PI2 - this->angle + a;
		float left = this->angle >= a ? this->angle - a : this->angle + M_PI2 - a;

		if (right < left) {
			return right;
		} else {
			// We return a negative angle if we must rotate to left
			return -left;
		}
	}
};

struct JavaSingleGame {
	Car car;
    int totalCheckpoints=0;
    int currentCheckpoint = 0;
    int timer=0;
    double colTime = 0.0;
    bool isDone = false;
    vector<Unit> *checkpoints;
    int maxTimer = 1600;
    
    JavaSingleGame() {
    }

    void clone(const JavaSingleGame &game) {
        car.x = game.car.x;
        car.y = game.car.y;
        car.vx = game.car.vx;
        car.vy = game.car.vy;
        car.angle = game.car.angle;
        car.debug = game.car.debug;
        car.prevAngle = game.car.prevAngle;
        car.target.x = game.car.target.x;
        car.target.y = game.car.target.y;
        car.thrust = game.car.thrust;
        car.friction = game.car.friction;
        car.mass = game.car.mass;
        totalCheckpoints = game.totalCheckpoints;
        currentCheckpoint = game.currentCheckpoint;
        timer = game.timer;
        colTime = game.colTime;
        isDone = game.isDone;
        checkpoints = game.checkpoints;
        maxTimer = game.maxTimer;
    }

	void init(vector<Unit> *cpts) {
        checkpoints = cpts;
        car = Car(checkpoints->at(0).x, checkpoints->at(0).y, 0);
        car.prevAngle = car.getAngle((*checkpoints)[1]);
        car.angle = car.prevAngle;
        car.adjust();
        totalCheckpoints = checkpoints->size()*1;
	}



    void tick(int x, int y, int thrust)  {
        if(isDone) {
            return;
        }
        car.handleInput(x, y, thrust);
        checkCollisions();
        car.adjust();

        if(!isDone) timer++;
        if(timer == maxTimer && !isDone){
            isDone = true;
        }
    }

    void checkCollisions(){
        bool hasCollided = true;
        double t = 0.0;
        colTime = 2.0;
        while(!isDone && hasCollided){
            hasCollided = false;
            double col_time = car.getCollision(checkpoints->at((currentCheckpoint+1) % checkpoints->size()),
                                               CheckpointRadius);
            if(col_time >= 0.0 && col_time + t <= 1.0){
                hasCollided = true;
                currentCheckpoint++;
                t += col_time;
                colTime = t;
                car.move(col_time);
                
                if(DEBUG_COLLISION) {
                    cerr << "collision! Time = " << col_time << "  cpid = " << currentCheckpoint<< endl;
                }
                
                if(currentCheckpoint >= totalCheckpoints){
                    isDone = true;
                }
            }
        }
        
        car.move(1.0-t);
    }

    Unit getNextCheckpoint(){
        return checkpoints->at((currentCheckpoint+1) % checkpoints->size());
    }

    Unit getNextNextCheckpoint() {
        return checkpoints->at((currentCheckpoint+2) % checkpoints->size());
    }

};
// ========= // Game Engine


class FitnessGame : public FitnessFunction {
public:
	vector<Unit> *mCkpts;
    JavaSingleGame *mGameInit;
    bool mForceState;
	int mMaxTicks;
    int mMaxElapsed;
    chrono::high_resolution_clock::time_point mTimerStart;

    int nSteps;

    void setTimerStart(chrono::high_resolution_clock::time_point start) {
        mTimerStart = start;
    }

    void setMaxElapsed(int maxElapsed) {
        mMaxElapsed = maxElapsed;
    }

	FitnessGame(vector<Unit> *ckpts, int nsteps) {
		mCkpts = ckpts;
        mForceState = false;
        mMaxTicks = 1000000;
        mMaxElapsed = 1000;
        mTimerStart = chrono::high_resolution_clock::now();
        nSteps = nsteps;
	}

    ~FitnessGame() {

    }

    void forceState(JavaSingleGame *gameInit) {
        mGameInit = gameInit;
        mForceState = true;
    }

    void setMaxTicks(int maxTicks) {
        mMaxTicks = maxTicks;
    }

	float eval(const RealChromosome &g) {
        // chromosome = [c_angle_0, c_angel_1, ..., c_angle_nSteps,  Thrust_0, Thrust_1, ... ,Thrust_nSteps]
		double fitness = 0;
        double dx, dy;
        int c_x, c_y, thrust;
        Unit nextCheckpoint;

        double elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - mTimerStart).count();
        int tick = 0;
        if (elapsed > mMaxElapsed) {
            //cerr << "happens " << elapsed << endl;
            return 1000;
        }
        
        JavaSingleGame game;

        if (mForceState) {
            game.clone(*mGameInit);
        } else {
            game.init(mCkpts);
        }

        int currentCpk = game.currentCheckpoint;
        for(int i=0; i<nSteps; i++) {
            c_x = game.car.x + 10000*cos(g.gene[i]);
            c_y = game.car.y + 10000*sin(g.gene[i]);
            game.tick(c_x, c_y, g.gene[i+nSteps]);
            
            nextCheckpoint = game.getNextCheckpoint();

            dx = (game.car.x - nextCheckpoint.x)/1600;
            dy = (game.car.y - nextCheckpoint.y)/1600;

            if(currentCpk != game.currentCheckpoint) {
                fitness -= 100;
                currentCpk = game.currentCheckpoint;
            }

            fitness += 0.1*(dx*dx + dy*dy);

            if(game.isDone || tick<mMaxTicks) break;
            tick++;
        }

		return (float)fitness;
	}
};


int main()
{
    int checkpointCount;
    JavaSingleGame game;
    int control_x, control_y, control_thrust;

    checkpointCount = 9;
    vector<double> ckptsX = {2757, 3358, 10353, 2757, 3358, 10353, 2757, 3358, 10353};
    vector<double> ckptsY = {4659, 2838, 1986, 4659, 2838, 1986, 4659, 2838, 1986};

    vector<Unit> cps(checkpointCount);
    for (int i = 0; i < checkpointCount; i++) {
        cps[(i+1) % checkpointCount].x = ckptsX[i];
        cps[(i+1) % checkpointCount].y = ckptsY[i];
    }

    // game loop
    game.init(&cps);

    // ====================================== First optimization
    int nSteps = 3;
    RealGA realga;
    RealGAOptions options;
    FitnessGame fitnessFunc(&cps, nSteps);
    

    vector<float> LB(2*nSteps), UB(2*nSteps);
    // c_angle
    for(int i=0;i<nSteps;i++) {
        LB[i] = 0;
        UB[i] = M_PI2;
    }
    // thrust
    for(int i=nSteps;i<2*nSteps;i++) {
        LB[i] = 0;
        UB[i] = 200;
    }

    options.setChromosomeSize(2*nSteps);
    options.setBounds(LB, UB);
    options.setPopulationSize(50);
    options.setMutationType("uniform");
    options.setMutateDuplicatedFitness(false);
    options.setSelectionType("roulette");
    options.setMutationRate(0.02);

    realga.init(options, &fitnessFunc, false);
    realga.popInitRandUniform();

    cerr << "Start Game !" << endl;

    while (1) {
        auto time_start_cycle = std::chrono::high_resolution_clock::now();
// ================================================= GA Optimization =======================
        fitnessFunc.forceState(&game);
        fitnessFunc.setTimerStart(time_start_cycle);

        int evolutions = 0;
        double elapsed;
        double max_elapsed = 40;
        while(1) {
            realga.evolve();
            
            evolutions++;
            elapsed = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - time_start_cycle).count();
            cerr << "elapsed " << elapsed << endl;
            if (elapsed >= max_elapsed) break;
        }

        RealChromosome bestSolution;
        bestSolution = realga.getBestChromosome();
        cerr << "Num evolutions: " << evolutions << endl;
        cerr << "Best fitness: " << bestSolution.fitness << endl;

        control_x = game.car.x + 1000*cos(bestSolution.gene[0]);
        control_y = game.car.y + 1000*sin(bestSolution.gene[0]);
        control_thrust = bestSolution.gene[nSteps];
        //cerr << realga.populationToString() << endl;

        
        cerr << "game cycle time " <<  chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - time_start_cycle).count() << endl;

        game.tick(control_x, control_y, control_thrust);
    }

}