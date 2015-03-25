#include <memory>
#include <utility>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "DFAutomaton.h"

#include "RegexState.h"
#include "IRegex.h"
#include "PhiRegex.h"
#include "LiteralRegex.h"
#include "EpsilonRegex.h"
#include "UnionRegex.h"
#include "ConcatRegex.h"
#include "ClosureRegex.h"
#include "HashExtensions.h"

typedef std::string State;
typedef std::string Char;


std::shared_ptr<IRegex> concatRE( std::shared_ptr<IRegex> left, std::shared_ptr<IRegex> right){
	if( left->ToString() == "\\e"){
		return right;
	}else if ( right->ToString() == "\\e"){
		return left;
	}else if ( left->ToString() == "\\p" || right->ToString() == "\\p"){
		return std::make_shared<PhiRegex>();
	}
	
	return std::make_shared<ConcatRegex>(left, right);
}

std::shared_ptr<IRegex> unifyRE( std::shared_ptr<IRegex> left, std::shared_ptr<IRegex> right){
	if( left->ToString() == "\\p"){
		return right;
	}else if ( right->ToString() == "\\p"){
		return left;
	}else if( right->ToString() == left->ToString()){
		return left;
	}
	
	return std::make_shared<UnionRegex>(left, right);
}

std::shared_ptr<IRegex> closeRE( std::shared_ptr<IRegex> in ){
	if( in->ToString() == "\\e" || in->ToString() == "\\p" ){
		return std::make_shared<EpsilonRegex>();
	}
	return std::make_shared<ClosureRegex>(in);
}


class REAutomaton
{
private:
	std::vector<State> states;
	State startState;
	std::vector<State> acceptStates;
	
	std::unordered_map<State, std::vector<std::pair<std::shared_ptr<IRegex>, State>>> transitionMap;
	
public:
	REAutomaton(DFAutomaton<std::string, std::string>& DFA){	// Create an REAutomaton that mirrors an existing DFA
		for( State state : DFA.GetStates().getItems()){
			states.push_back(state);
			for( Char trans: DFA.GetAlphabet().getItems()){
				if(DFA.PerformTransition(state, trans) != ""){
					transitionMap[state].push_back( std::pair<std::shared_ptr<IRegex>, State> (std::make_shared<LiteralRegex>(trans),  DFA.PerformTransition(state, trans)));
				}
			}
		}
		
		startState = DFA.getStartState();
		
		for( State state : DFA.getAcceptingStates().getItems()){
			acceptStates.push_back(state);
		}
	}
	
	REAutomaton( REAutomaton* originalREA, State acceptState){	// Create a duplicate REAutomaton that only has a single accept state
		this-> states = originalREA-> states;
		this-> startState = originalREA-> startState;
		this-> transitionMap = originalREA-> transitionMap;
		
		this-> acceptStates.push_back(acceptState);
	}
	
	std::shared_ptr<IRegex> toRE(){
		
		std::shared_ptr<IRegex> finalRegex = std::make_shared<PhiRegex>();
		
		for( State acceptState: acceptStates ){		// In a sub-REAutomaton with only one accept state
			REAutomaton REA( this, acceptState);
			
			// Reduce the REA to 2 states
			
			while( REA.states.size() > 2 || (REA.states.size() == 2 && REA.startState == REA.acceptStates[0])){
				
				State s;
				for( State toet : REA.states ){
					if( toet != REA.startState && toet != REA.acceptStates[0]) s = toet;
				}
				
					// Find S and pairs of (P, p): delta(s, S) == s and delta(s, P) == p
				
				std::shared_ptr<IRegex> S = std::make_shared<PhiRegex>();
				std::vector<std::pair<std::shared_ptr<IRegex>, State>> pPairs;
				
				
				for( auto transitionPair : REA.transitionMap[s]){
					if( transitionPair.second == s){
						S = unifyRE(S, transitionPair.first);
					}else{
						pPairs.push_back(transitionPair);
					}
				}
			
					// Find pairs of (Q, q): delta(s, Q) = q
					
				for( State q: REA.states ){
					if( q != s ){
						
						std::vector<std::pair<std::shared_ptr<IRegex>, State>> qPairsNew;
						
						for( auto qPair : REA.transitionMap[q] ){
							if( qPair.second == s){					// (Q, q): delta(s, Q) = q
								
								for( auto pPair: pPairs ){
									
									// Find R
									
									std::shared_ptr<IRegex> R = std::make_shared<PhiRegex>();
									for( auto qpPair : REA.transitionMap[q]){
										if( qpPair.second == pPair.second){
											R = unifyRE(R, qpPair.first);
										}
									}
									
									std::shared_ptr<IRegex> newRE = qPair.first;	// Q
									newRE = concatRE(newRE, closeRE(S));			// QS*
									newRE = concatRE(newRE, pPair.first);			// QS*P
									newRE = unifyRE( R, newRE);						// R + QS*P
									
									qPairsNew.push_back( std::pair<std::shared_ptr<IRegex>, State> (newRE, pPair.second));
						
								}
							}else{	// qPair.Second != s
								qPairsNew.push_back(qPair);
							}
						}
						
						if(qPairsNew.size() > 1){
							for( auto it = qPairsNew.begin(); it != qPairsNew.end()-1; it++){
								if (it->second == q){
									for( auto subit = it+1; it != qPairsNew.end(); subit++){
										if( subit->second == q){
											qPairsNew.erase(subit);
											break;
										}
									}
								}
							}
						}
						
						REA.transitionMap[q] = qPairsNew;
						
					}
				}
				
				// Dirty work: remove every trace of S
				
				REA.states.erase( std::find(REA.states.begin(), REA.states.end(), s));	// remove the state s
				
				REA.transitionMap.erase(s);	// remove every transition from s
				
				
			}
			// Interpret the reduced REA:
			
			std::shared_ptr<IRegex> R = std::make_shared<PhiRegex>();	// start -> start
			std::shared_ptr<IRegex> S = std::make_shared<PhiRegex>(); 	// start -> end
			
			
			for( auto startDouble : REA.transitionMap[REA.startState]){
				if( startDouble.second == REA.startState){
					R = startDouble.first;
				} else if(startDouble.second != ""){
					S = startDouble.first;
				}
			}
			
			std::shared_ptr<IRegex> U = std::make_shared<PhiRegex>();	// end -> end
			std::shared_ptr<IRegex> T = std::make_shared<PhiRegex>(); 	// end -> start
			
			for( auto endDouble : REA.transitionMap[REA.acceptStates[0]]){
				if( endDouble.second == REA.acceptStates[0]){
					U = endDouble.first;
				} else if(endDouble.second != ""){
					T = endDouble.first;
				}
			}
			
			std::shared_ptr<IRegex> thisRegex = std::make_shared<PhiRegex>();
			
			if( REA.transitionMap.size() > 1){
				thisRegex = concatRE(closeRE(unifyRE(R, concatRE(concatRE( S , closeRE(U)),T))), concatRE(S, closeRE(U)));
			} else {
				thisRegex = closeRE(R);
			}
			finalRegex = unifyRE(finalRegex, thisRegex);
		}
		
		return finalRegex;
	}
};

std::shared_ptr<IRegex> DFAtoRE(DFAutomaton<std::string, std::string>& DFA){
	
	REAutomaton REA( DFA );
	
	auto pointer = REA.toRE();
	
	return pointer;

}