#include "../head/TestADMMConst1.h"

int testADMM1()
{
	int n = 1;
	if (!testADMM1Contruct1()) return n;
	n++;
	if (!testADMM1Contruct2()) return n;
	n++;
	if (!testADMM1Contruct3()) return n;
	n++;
	if (!testADMM1Function1()) return n;
	n++;
	if (!testADMM1Function2()) return n;
	n++;
	if (!testADMM1Function3()) return n;
	n++;
	if (!testADMM1Function4()) return n;
	n++;
	if (!testADMM1Function5()) return n;
	n++;
	if (!testADMM1Function6()) return n;
	n++; // 10
	if (!testADMM1Solve1()) return n;
	n++;
	if (!testADMM1Solve2()) return n;
	n++;
	if (!testADMM1Solve3()) return n;
	n++;
	

	return 0;
}

bool testADMM1Contruct1()
{
	std::cout << "default constructor" << std::endl;
	ADMMConst1 a;
	return true;
}

bool testADMM1Contruct2()
{
	float rho = 2;

	std::cout << "param constructor" << std::endl;
	ADMMConst1 a(rho);
	return true;
}
bool testADMM1Contruct3()
{
	float rho = 2;

	std::cout << "2 times constructor" << std::endl;
	ADMMConst1 a;
	a = ADMMConst1(rho);
	return true;
}

bool testADMM1Solve1()
{
	//solve(Simparam* result, Simparam sim, StudyCase cas);
	StudyCase cas;
	cas.Set2node();
	int nAgent = cas.getNagent();
	
	Simparam param(nAgent, 1);
	/*int iterG = 1;
	int iterL = 5;
	param.setItG(iterG);
	param.setItL(iterL);*/
	Simparam res(param);

	ADMMConst1 a;

	MatrixCPU Trade(nAgent, nAgent);
	Trade.set(0, 1, -1);
	Trade.set(1, 0, 1);
	a.solve(&res, param, cas);

	MatrixCPU trade = res.getTrade();
	res.display();
	trade.display();
	return trade.isEqual(&Trade,0.001);
}
bool testADMM1Solve2()
{
	//solve(Simparam* result, Simparam sim, StudyCase cas);
	StudyCase cas;
	cas.Set29node();
	
	int nAgent = cas.getNagent();

	Simparam param(nAgent, cas.getNLine());
	float epsG = 0.00001f;
	param.setEpsG(epsG);
	param.setRho(10000);
	Simparam res(param);
	ADMMConst1 a;
	a.solve(&res, param, cas);
	
	res.display();
	
					
	float Pn[31] = { -1.008853555,-4.62966156,-2.927534103,-0.8979898691,-0.9462603927,-0.09805059433,-0.127968356,-4.168303013,-3.151874542,-2.261414766,-0.670329392,-3.399893284,-0.4841034412,-2.775528431,-3.008597374,-1.849177122,-0.5534118414,-2.362840891,-1.122991204,-0.1379692554,-2.332088947,4.406820297,5.406073093,3.676487684,3.929354668,4.570535183,2.529039145,3.478654861,2.755935192,3.768760443,4.393183708, };

	MatrixCPU P(31, 1);
	for (int i = 0; i < 31; i++) {
		P.set(i, 0, Pn[i]);
	}

	MatrixCPU P2 = res.getPn();
	std::cout << std::endl;
	return P2.isEqual(&P,0.01);
}

bool testADMM1Solve3()
{
	//solve(Simparam* result, Simparam sim, StudyCase cas);
	StudyCase cas;
	float lim = 0.8;
	cas.Set2nodeConstraint(lim);
	int nAgent = cas.getNagent();
	float epsG = 0.00001f;
	int iterG = 1000;
	Simparam param(nAgent, 1);
	param.setEpsG(epsG);
	param.setEpsGC(epsG);
	param.setItG(iterG);
	Simparam res(param);
	

	float value = (1 - lim) * (lim > 1) + lim;

	ADMMConst1 a;

	MatrixCPU Trade(nAgent, nAgent);
	Trade.set(0, 1, -value);
	Trade.set(1, 0, value);
	a.solve(&res, param, cas);

	MatrixCPU trade = res.getTrade();
	res.display();
	trade.display();
	return trade.isEqual(&Trade, 0.001);
}


bool testADMM1Function1()
{
	/*
void ADMMConst::updateBt1(MatrixCPU* Bt1, MatrixCPU* trade, float rho, MatrixCPU* LAMBDA)
{
	Bt1->set(trade);
	Bt1->subtractTrans(trade);
	Bt1->multiply(0.5*rho); // c'est mieux que de copier une matrix compl�te
	Bt1->subtract(LAMBDA);
	Bt1->divide(rho);

}*/

	int nTrade = 2;
	float value1 = 2;
	float value2 = 4;
	float value3 = -2;
	float value4 = -3.5;
	float rho = 1.5;
	MatrixCPU Bt1(nTrade, 1, 0);
	
	MatrixCPU Bt11(nTrade, 1, value1);
	Bt11.set(0, 0, 0.5 * (value2 - value4) - value3 / rho);
	Bt11.set(1, 0, 0.5 * (value4 - value2) - value3 / rho);
	MatrixCPU trade(nTrade, 1, value2);
	trade.set(1, 0, value4);
	MatrixCPU LAMBDA(nTrade, 1, value3);
	MatrixCPU CoresLinTrans(nTrade, 1);
	CoresLinTrans.set(0, 0, 1);
	CoresLinTrans.set(1, 0, 0);


	for (int t = 0; t < nTrade; t++) {
		int k = CoresLinTrans.get(t, 0);
		Bt1.set(t, 0, trade.get(t, 0) - trade.get(k, 0));
	}
	Bt1.multiply(0.5 * rho);
	Bt1.subtract(&LAMBDA);
	Bt1.divide(rho);
	

	return Bt1.isEqual(&Bt11);
}

bool testADMM1Function2()
{
	/*void ADMMConst::updateBt2(MatrixCPU* Bt2, MatrixCPU* Tlocal, MatrixCPU* Tmoy, MatrixCPU* P, MatrixCPU* MU)
{
	Bt2->set(Tlocal);
	Bt2->subtractVector(Tmoy);
	Bt2->addVector(P);
	Bt2->subtractVector(MU);
}*/
	int nAgent = 2;
	int nTrade = 2;
	float value1 = 0;
	float value2 = 2;
	float value3 = 3.5;
	float value4 = -4;
	float value5 = 4;
	
	MatrixCPU Bt22(nTrade, 1, value2-value3-value5+value4);
	MatrixCPU Bt2(nTrade, 1, 0);
	MatrixCPU Tlocal(nTrade, 1, value2);
	MatrixCPU Tmoy(nAgent, 1, value3);
	MatrixCPU P(nAgent, 1, value4);
	MatrixCPU MU(nAgent, 1, value5);
	MatrixCPU nVoisin(nAgent, 1, 1);
	MatrixCPU CoresAgentLin(nAgent, 1, 0);
	CoresAgentLin.set(1, 0, 1);

	
	for (int i = 0; i < nAgent; i++) {
		int nVoisinLocal = nVoisin.get(i, 0);
		int beginLocal = CoresAgentLin.get(i, 0);
		int endLocal = beginLocal + nVoisinLocal;
		for (int j = beginLocal; j < endLocal; j++) {
			float m = Tlocal.get(j, 0) - Tmoy.get(i, 0) + P.get(i, 0) - MU.get(i, 0);
			Bt2.set(j, 0, m);
		}
	}

	Bt2.display();
	Bt22.display();

	return Bt2.isEqual(&Bt22);
}

bool testADMM1Function3()
{
	/*float ADMMConst::calcRes( MatrixCPU* Tlocal, MatrixCPU* Tlocal_pre, MatrixCPU* Tmoy, MatrixCPU* P)
{
	MatrixCPU temp(*Tlocal);
	temp.subtract(Tlocal_pre);

	MatrixCPU temp2(*Tmoy);
	temp2.subtract(P);
	float d1 = temp.max2();
	float d2 = temp2.max2();

	return d1 * (d1 > d2) + d2 * (d2 >= d1);
}*/
	int nTrade = 4;
	int nAgent = 3;
	float value1 = 2;
	float value2 = 4;
	float value3 = -2;
	float value4 = 3;
	MatrixCPU Tlocal(nTrade, 1, value1);
	MatrixCPU Tlocal_pre(nTrade, 1, value2);
	MatrixCPU Tmoy(nAgent, 1, value3);
	MatrixCPU P(nAgent, 1, value4);
	MatrixCPU temp(Tlocal);
	temp.subtract(&Tlocal_pre);

	MatrixCPU temp2(Tmoy);
	temp2.subtract(&P);
	float d1 = temp.max2();
	float d2 = temp2.max2();
	
	float d = d1 * (d1 > d2) + d2 * (d2 >= d1);
	float d11 = fabs(value1 - value2);
	float d22 = fabs(value3 - value4);
	


	
	return (d==(d11 * (d11 > d22) + d22 * (d22 >= d11)));
}

bool testADMM1Function4()
{
	/*
	void ADMMConst::updateTl(MatrixCPU* Tlocal, float at1, float at2, MatrixCPU* Bt1, MatrixCPU*Bt2, MatrixCPU* Ct, MatrixCPU* matLb, MatrixCPU* matUb)
{

	float ada = at1 / at2; // pourrait �tre pr�calcul�
	float apa = at1 + at2;

	Tlocal->set(Bt1);
	Tlocal->multiply(ada);
	Tlocal->add(Bt2);
	Tlocal->multiply(at2);
	Tlocal->subtract(Ct);
	Tlocal->divide(apa); 
	Tlocal->project(matLb, matUb);
}*/

	int nAgent = 3;
	int nTrade = 4;
	float value1 = 2;
	float value2 = -8;
	float value3 = 1;
	float value4 = 1;
	float value5 = -1;
	float value6 = -30;
	float value7 = 0;
	float value8 = (value1 * value3 + value2 * value4 - value5) / (value3 + value4);
	if (value8 > value7) {
		value8 = value7;
	}
	else if (value8 < value6) {
		value8 = value6;
	}
	MatrixCPU Bt1(nTrade,1,value1);
	float at1 = value3;
	MatrixCPU Bt2(nTrade, 1, value2);
	float at2 = value4;
	MatrixCPU Ct(nTrade, 1, value5);
	MatrixCPU Lb(nTrade, 1, value6);
	MatrixCPU Ub(nTrade, 1, value7);
	MatrixCPU Tlocal(nTrade, 1);
	MatrixCPU Tlocal2(nTrade, 1);
	
	float ada = at1 / at2; 
	float apa = at1 + at2;

	Tlocal2.set(&Bt1);
	Tlocal2.multiply(ada);
	Tlocal2.add(&Bt2);
	Tlocal2.multiply(at2);
	Tlocal2.subtract(&Ct);
	Tlocal2.divide(apa);
	Tlocal2.project(&Lb, &Ub);
	
	
	MatrixCPU test(nTrade, 1, value8);

	return (Tlocal2.isEqual(&test));
}

bool testADMM1Function5()
{
	/*
	void ADMMConst::updateP(MatrixCPU* P, MatrixCPU* Ap1, MatrixCPU* Ap12, MatrixCPU* Bp1, MatrixCPU* Cp, MatrixCPU* Pmin, MatrixCPU* Pmax)
{
	P->multiplyT(Ap1, Bp1);
	P->subtract(Cp);
	
	P->divideT(Ap12);
	P->project(Pmin, Pmax);
}*/
	int nAgent = 2;
	float value1 = 0.5;
	float value2 = 1;
	float value3 = 1;
	float value5 = 8;
	float value6 = -30;
	float value7 = 0;
	float value8 = (value1 * value2 - value5) / (value3 + value2);
	if (value8 > value7) {
		value8 = value7;
	}
	else if (value8 < value6) {
		value8 = value6;
	}
	MatrixCPU Bp1(nAgent, 1, value1);
	MatrixCPU Ap1(nAgent, 1, value2);
	MatrixCPU Ap2(nAgent, 1, value3);
	MatrixCPU Ap12(nAgent, 1, value3);
	Ap12.add(&Ap1, &Ap2);
	MatrixCPU Cp(nAgent, 1, value5);
	MatrixCPU Lb(nAgent, 1, value6);
	MatrixCPU Ub(nAgent, 1, value7);
	MatrixCPU P(nAgent, 1);
	P.multiplyT(&Ap1, &Bp1);
	P.subtract(&Cp);

	P.divideT(&Ap12);
	P.project(&Lb, &Ub);
	
	MatrixCPU test(nAgent, 1, value8);
	return test.isEqual(&P);
}

bool testADMM1Function6()
{
	//LAMBDA
	int nAgent = 3; // 2 conso et un prod
	int ntrade = 4;
	float value1 = 2;
	float value2 = -8;
	float value3 = 1.5;
	float value4 = 4;
	float rho = value3;

	MatrixCPU LAMBDALin(ntrade, 1, value1);
	MatrixCPU trade(ntrade, 1, value2);
	trade.set(2, 0, value4);
	trade.set(3, 0, value4);
	MatrixCPU CoresLinTrans(ntrade, 1);
	CoresLinTrans.set(0, 0, 2);
	CoresLinTrans.set(1, 0, 3);
	CoresLinTrans.set(2, 0, 0);
	CoresLinTrans.set(3, 0, 1);

	MatrixCPU LAMBDALin2(ntrade, 1, value1 + 0.5 * value3 * (value2 + value4));
	
	for (int t = 0; t < ntrade; t++) {
		int k = CoresLinTrans.get(t, 0);
		float lamb = 0.5 * rho * (trade.get(t, 0) + trade.get(k, 0));
		LAMBDALin.set(t, 0, LAMBDALin.get(t, 0) + lamb);
	}


	return (LAMBDALin.isEqual(&LAMBDALin2));
}