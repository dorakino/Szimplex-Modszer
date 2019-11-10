#include <iostream>
#include <vector>

using namespace std;

class Problema{
private:
    vector <vector<float> > A; //egyutthato matrix
    vector<float> r;   // relaciok vektora
    vector<float> b;   //korlatok vektora
    vector<float> c;   //eredeti celfv egyutthatoinak vektora
    int m; //feltetelszam
    int n; // dimenzioszam

    vector <vector<float> > matrixElsoFazis; //az új mátrix, amiben számolhatok az első fázis során
    vector <vector<float> > matrixMasodikFazis; // a 2. fazis mátrixa

    vector< vector<float> > aktualisBazis;

    bool kellElso  = false;
    bool kellMasodik  = true;

        // segedvaltozok:
    int sorSzamElsoFazis;
    int oszlopSzamElsoFazis;
    int sorSzamMasodikFazis;
    int oszlopSzamMasodikFazis;

    int uOszlopIndexe;
    int utolsoOszlopI;
    vector<int> csillagosSorokIndexei = {};
    vector< float > kikerultOszlopokIndexei = {};

    int ideiglenes;
    int celfvSorindexe = -1;
    int megszamoltElemekSzama = 0;

        // p*q -s vektorok vektora, minden eleme 0
	vector<vector<float> > uresVektorokVektoraKeszites(int p, int q){
	    vector<vector<float> > uresVV = {};
	    vector<float> uresV = {};
        for(int i = 0; i < p; i++){
            uresV = {};
            for(int j = 0; j < q; j++){
                uresV.push_back(0);
            }
        uresVV.push_back(uresV);
        }
        return uresVV;
	};

    vector< vector<float> > elemekKinullazasaAdottElemmel(int pivotSorIndex, int pivotOszlopIndex, vector< vector<float> >& mtx){
        int sorSzam = mtx.size();
        int oszlopSzam = mtx[0].size();
        float elsoErtek;
	    float masodikErtek;
	    float temp;

	    for(int i=0; i < sorSzam;i++){
            if(i == pivotSorIndex){
                for(int j = 0; j < oszlopSzam; j++){
                    if(pivotOszlopIndex == j){
                        elsoErtek = mtx[i][j];
                    }
                }
                for(int j = 0; j < oszlopSzam; j++){
                    mtx[i][j] /= elsoErtek;
                }
            }
        }

        for(int i = 0; i < sorSzam; i++){
            if(i != pivotSorIndex && mtx[i][pivotOszlopIndex] != 0){

                masodikErtek = mtx[i][pivotOszlopIndex] / mtx[pivotSorIndex][pivotOszlopIndex];
                for(int j = 0; j < oszlopSzam; j++){
                    temp = mtx[pivotSorIndex][j] * masodikErtek;
                    mtx[i][j] -= temp;
                }
            }
        }
        return mtx;
	};

	int legkisebbHanyadosKereso(vector< vector< float > >& mtx, int negativosOszlopIndexe, int vizsgalandoSorokSzama){
	    int aktualisSorIndex = -1;
	    int oszlopokSzama = mtx[celfvSorindexe].size();
	    int utolsoOszlopIndexe = oszlopokSzama - 1;
	    int aktualisLegkisebbHanyados = 1000;
	    bool vanPozitivElem = false;
	    for(int i = 0; i < vizsgalandoSorokSzama; i++){
            if(mtx[i][negativosOszlopIndexe] > 0){
                vanPozitivElem = true;
                if(mtx[i][utolsoOszlopIndexe] / mtx[i][negativosOszlopIndexe] < aktualisLegkisebbHanyados){
                    aktualisSorIndex = i;
                    aktualisLegkisebbHanyados = mtx[i][utolsoOszlopIndexe] / mtx[i][negativosOszlopIndexe];
                }
            }
	    }

	    if(vanPozitivElem == false){
            cout << "Nincsen pozitiv elem a vizsgalt oszlopban." << endl;
	    }
	    return aktualisSorIndex;
	};

	vector< vector<float> > bazisLekerese(vector< vector<float> >& mtx) {
        int egyesekSzama = 0;
        int nullakSzama = 0;
        int oszlopokSzama = mtx[0].size();
        int sorokSzama = mtx.size();
        vector<float> tempVector = {};
        int tempSorindex = 0;
        vector< vector<float> > bazis = {};

        for(int i = 0; i < oszlopokSzama - 1; i++){
            egyesekSzama = 0;
            nullakSzama = 0;
            tempVector = {};
            if(i != uOszlopIndexe){
                for(int j = 0; j < m; j++){
                    if(mtx[j][i] == 0){
                        nullakSzama++;
                    }
                    if(mtx[j][i] == 1){
                        egyesekSzama++;
                        tempSorindex = j;
                    }
                }
            }
            if(egyesekSzama == 1 && nullakSzama == (m - 1)){
                tempVector.push_back(i);
                tempVector.push_back(tempSorindex);
                bazis.push_back(tempVector);
            }
        }
        return bazis;
	};
        // elsoNegativElemKereso fv kap:
        // Egy mátrixot,
        // azt a sorindexet, amely sorában keresem az első negatív elemet, azon oszlopok közül, amelyek még benne vannak a mátrixban

        // Visszatér a megfelelő oszlop indexével, ha ilyen van
        // Szól, ha nincs ilyen, és -1-gyel tér vissza

	int elsoNegativElemKereso(vector< vector< float > >& mtx, int celfvSorindexe){
	    int aktualisOszlopIndex = -1;
	    int oszlopokSzama = mtx[celfvSorindexe].size();
        int k = 0;
        bool vanTalalt = false;
        bool tempTalalt = false;

        while(!vanTalalt && k < oszlopokSzama - 1){
            if(mtx[celfvSorindexe][k] >= 0){
                k++;
                megszamoltElemekSzama++;
            }
            if(mtx[celfvSorindexe][k] < 0){
                for(int i = 0; i < kikerultOszlopokIndexei.size(); i++){
                    if(k == kikerultOszlopokIndexei[i]){
                        tempTalalt = true;
                    }
                }
                if(tempTalalt){
                    k++;
                    megszamoltElemekSzama++;
                }
                if(!tempTalalt){
                    vanTalalt = true;
                }
            }

        }
        if(k != oszlopokSzama - 1){
            aktualisOszlopIndex = k;
        }
        else{
            cout << "Nincsen tobb negativ elem a vizsgalt sorban." << endl << endl;
            aktualisOszlopIndex = -1;
        }
        return aktualisOszlopIndex;
	};

	void oszlopKivetele(vector< vector< float > >& mtx){
	    int kikeruloOszlopIndexe = -1;
	    //vector< vector< float > > tempBazis = {};
	    vector< vector< float > > tempMtx = {};
	    vector< float > nincsBazisbanIndexek = {};
	    //tempBazis = bazisLekerese(matrixElsoFazis);
	    bool benneVan;

	    for(int i = 0; i < oszlopSzamElsoFazis - 1; i++){
            benneVan = false;
            for(int j = 0; j < aktualisBazis.size(); j++){
                if(aktualisBazis[j][0] == i){
                    benneVan = true;
                }
            }
            if(!benneVan && i != uOszlopIndexe){
                nincsBazisbanIndexek.push_back(i);
            }
	    }

	    benneVan = false;
	    int kivevendoOszlopIndexe = -1;
	    for(int i = 0; i < csillagosSorokIndexei.size(); i++){
            if(csillagosSorokIndexei[i] == 1){
                benneVan = false;
                for(int j = 0; j < nincsBazisbanIndexek.size(); j++){
                    if(n + 1 + i == nincsBazisbanIndexek[j]){
                        for(int p = 0; p < kikerultOszlopokIndexei.size(); p++){
                            if(kikerultOszlopokIndexei[p] == n + 1 + i){
                                benneVan = true;
                            }
                        }
                        if(!benneVan){
                            kivevendoOszlopIndexe = n + 1 + i;
                            kikerultOszlopokIndexei.push_back(kivevendoOszlopIndexe);
                            megszamoltElemekSzama++;
                        }
                    }
                }
            }
	    }
	    if(kivevendoOszlopIndexe != -1){
            cout << "ignoraljuk a " << kivevendoOszlopIndexe << "-s indexu oszlopot a matrixbol." << endl;
	    }
	};

	void elsoFazishozMtxKeszites(){

	    matrixElsoFazis = uresVektorokVektoraKeszites(sorSzamElsoFazis, oszlopSzamElsoFazis);

	    utolsoOszlopI = n + m + 1;
	    uOszlopIndexe = n;

	    for(int i = 0; i < m; i++){
            csillagosSorokIndexei.push_back(0);
	    }

	    // ÉRTÉKADÁS
        for(int i = 0; i < m; i++){
            // A ertekeinek bevetele
            for(int j = 0; j < uOszlopIndexe; j++){
                matrixElsoFazis[i][j] = A[i][j];
            }
            //b ertekeinek bevetele
            matrixElsoFazis[i][oszlopSzamElsoFazis - 1] = b[i];

            // u oszlop
            if(r[i] == -1){
                matrixElsoFazis[i][uOszlopIndexe] = -1;
            }
        }

        //slack valtozok id mtx-nak egyutthatoinak bevetele
        for(int i = 0; i < m; i++){
            for(int j = uOszlopIndexe + 1; j < oszlopSzamElsoFazis - 1 ; j++){
                if(i == (j - (n+1))){
                    matrixElsoFazis[i][j] = 1;
                }
            }
        }

        //eredeti celfv bevetele
        for(int j = 0; j < uOszlopIndexe; j++){
            matrixElsoFazis[m][j] = -c[j];
        }

        // sorok csillagozasa
        for(int i = 0; i < m; i++){
            if(r[i] != 1){
                csillagosSorokIndexei[i] = 1;
            }
        }

        //uj celfv bevetele
        for(int i = 0; i < uOszlopIndexe + 1; i++){
            for(int j = 0; j < m; j++){
                if(csillagosSorokIndexei[j] == 1){
                    matrixElsoFazis[m+1][i] += -matrixElsoFazis[j][i];
                }
            }
        }

        for(int j = 0; j < m; j++){
            if(csillagosSorokIndexei[j] == 1){
                matrixElsoFazis[m+1][utolsoOszlopI] += -matrixElsoFazis[j][utolsoOszlopI];
            }
        }
        aktualisBazis = bazisLekerese(matrixElsoFazis);
	};

	void MasodikFazishozMtxKeszites(){
	    if(kellElso){
            matrixMasodikFazis = uresVektorokVektoraKeszites(sorSzamMasodikFazis, oszlopSzamMasodikFazis);

            utolsoOszlopI = oszlopSzamMasodikFazis - 1;
            uOszlopIndexe = -1;

            // ÉRTÉKADÁS
            for(int i = 0; i < m + 1; i++){
                for(int j = 0; j < n; j++){
                    matrixMasodikFazis[i][j]=matrixElsoFazis[i][j];
                }
            }
            for(int i = 0; i < m + 1; i++){
                for(int j = n + 1; j < oszlopSzamElsoFazis; j++){
                    matrixMasodikFazis[i][j - 1] = matrixElsoFazis[i][j];
                }
            }
            aktualisBazis = bazisLekerese(matrixMasodikFazis);

	    }

	    else{
            oszlopSzamMasodikFazis = n + m + 1;
            sorSzamMasodikFazis = m + 1;
            matrixMasodikFazis = uresVektorokVektoraKeszites(sorSzamMasodikFazis, oszlopSzamMasodikFazis);
            utolsoOszlopI = oszlopSzamMasodikFazis - 1;
            uOszlopIndexe = -1;
            for( int i = 0; i < m; i++){
                for(int j = 0; j < n; j++){
                    matrixMasodikFazis[i][j] = A[i][j];
                }
                matrixMasodikFazis[i][oszlopSzamMasodikFazis - 1] = b[i];
            }
                //slack valtozok id mtx-nak egyutthatoinak bevetele
            for(int i = 0; i < m; i++){
                for(int j = n; j < oszlopSzamElsoFazis - 1 ; j++){
                    if(i == (j - (n+1))){
                        matrixMasodikFazis[i][j] = 1;
                    }
                }
            }
            for(int i = 0; i < c.size(); i++){
                matrixMasodikFazis[m][i] = -c[i];
            }
            aktualisBazis = bazisLekerese(matrixMasodikFazis);
	    }
	};

	void elsoFazis(){
	    int pivotSorI;
	    int pivotOszlopI;
        megszamoltElemekSzama = 0;

        celfvSorindexe = sorSzamElsoFazis - 1;

        elsoFazishozMtxKeszites();
        matrixKiirasa(matrixElsoFazis);
        bazisbanLevoOszlopokLekerese();
        cout << endl;

        while(1==1){
            pivotOszlopI = elsoNegativElemKereso(matrixElsoFazis, sorSzamElsoFazis - 1);
            if(pivotOszlopI == -1){
                cout << "Veget ert az elso fazis." << endl << endl;
                return;
            }
            pivotSorI = legkisebbHanyadosKereso(matrixElsoFazis, pivotOszlopI, sorSzamElsoFazis - 2);
            if(pivotSorI == -1){
                cout << "A celfv nem korlatos." << endl;
                kellMasodik = false;
                return;
            }
            matrixElsoFazis = elemekKinullazasaAdottElemmel(pivotSorI, pivotOszlopI, matrixElsoFazis);
            matrixKiirasa(matrixElsoFazis);

            aktualisBazis = bazisLekerese(matrixElsoFazis);
            bazisbanLevoOszlopokLekerese();

            oszlopKivetele(matrixElsoFazis);
            cout << endl;
        }
	};

	void masodikFazis(){
        int pivotSorI;
	    int pivotOszlopI;
        megszamoltElemekSzama = 0;
        celfvSorindexe = sorSzamMasodikFazis - 1;

        MasodikFazishozMtxKeszites();

        matrixKiirasa(matrixMasodikFazis);
        bazisbanLevoOszlopokLekerese();
        cout << endl;

        while(1==1){
            pivotOszlopI = elsoNegativElemKereso(matrixMasodikFazis, sorSzamMasodikFazis - 1);
            if(pivotOszlopI == -1){
                megoldasLekerese(matrixMasodikFazis);
                cout << "Veget ert a masodik fazis." << endl << endl;
                return;
            }
            pivotSorI = legkisebbHanyadosKereso(matrixMasodikFazis, pivotOszlopI, sorSzamMasodikFazis - 1);
            if(pivotSorI == -1){
                cout << "A celfv nem korlatos." << endl;
                return;
            }
            matrixMasodikFazis = elemekKinullazasaAdottElemmel(pivotSorI, pivotOszlopI, matrixMasodikFazis);
            matrixKiirasa(matrixMasodikFazis);

            aktualisBazis = bazisLekerese(matrixMasodikFazis);
            bazisbanLevoOszlopokLekerese();

            cout << endl;
        }
	};

public:
    Problema(const vector<vector<float> >& A0,
             const std::vector<float>& r0,
             const std::vector<float>& b0,
             const std::vector<float>& c0 ){
		A = A0;
		r = r0;
		b = b0;
		c = c0;
		m = A.size();
        n = A[0].size();
        sorSzamElsoFazis = m + 2;
        oszlopSzamElsoFazis = n + m + 2;
        oszlopSzamMasodikFazis = n + m + 1;
        sorSzamMasodikFazis = m+1;
	};

	    //tetszőleges matrix kiirasa
    void matrixKiirasa(vector< vector<float> >& mtx) const{
        int sorSzam = mtx.size();
        int oszlopSzam = mtx[0].size();

        for(int i = 0; i < sorSzam; i++){
            for(int j = 0; j < oszlopSzam; j++){
                cout << mtx[i][j]<<"   ";
            }
        cout << endl;
        }
    };

    void megoldasLekerese(vector< vector<float> >& mtx) {
        int egyesekSzama = 0;
        int nullakSzama = 0;
        int oszlopokSzama = mtx[0].size();
        int sorokSzama = mtx.size();
        vector<float> megoldas = {};

        for(int i = 0; i < oszlopokSzama - 1; i++){
            megoldas.push_back(0);
        }
        for(int i = 0; i < aktualisBazis.size(); i++){
            megoldas[ aktualisBazis[i][0] ] = mtx[ aktualisBazis[i][1] ][utolsoOszlopI];
        }

        cout << "Az optimalis megoldas: ";
        for(int i = 0; i < oszlopokSzama - 1; i++){
            cout << megoldas[i] << "  ";
        }
        cout << endl;
	};

	void bazisbanLevoOszlopokLekerese(){
	    cout << "Az aktualis bazisban levo oszlopok indexei:  ";
        for(int i = 0; i < aktualisBazis.size(); i++){
            cout << aktualisBazis[i][0] << "  ";
        }
        cout << endl;
	};

	void Simplex(){
	    for(int i = 0; i < r.size(); i++){
            if(r[i] != 1){
                kellElso = true;
            }
	    }
	    if(kellElso){
            elsoFazis();
	    }
	    if(kellMasodik){
            masodikFazis();
	    }
	};
};

int main()
{

    std::vector <std::vector<float> > A; //egyutthato matrix
    std::vector<float> r;     // relaciok vektora
    std::vector<float> b;   //korlatok vektora
    std::vector<float> c;   //celfv egyutthatoinak vektora

    /*
    "A" matrix: a valtozok egyutthatoinak matrixa
    "b" vektor: a korlatokat tartalmazza (az egyenlotlenseg rendszer "jobboldala")
    "c" vektor: a maximalizalando celfuggveny
    "r" vektor: a relaciokat tartalmazza, a kovetkezo hozzarendelessel:
                <= := 1
                =  := 0
                >= := -1
    Tovabba feltesszuk, hogy minden x_i valtozora, ahol i=1..n, x_i >= 0 teljesul.
    */

    int eset = 1;
    if(eset == 1){
        A = {
           {1, 0, 1, 1} ,
           {1, 1, 0, 2} ,
           {0, 1, 1, 0}
        };
        r = {0, 0, 1};
        b = {5, 6, 7};
        c = {1, 2, 4, 1};

    }

    if(eset == 2)
    {
        A = {
           {3, -2, 1} ,
           {1, 3, -4} ,
           {0, 1, 5} ,
           {1, 1, 1}
        };
        r = {1, 1, -1, 0};
        b = {5, 9, 1, 6};
        c = {-2, 3, -1};
    }

    if(eset == 3)
    {
        A = {
           {1, 1, 1} ,
           {2, 1, -1} ,
           {0, -1, 1}
        };
        r = {1, -1, -1};
        b = {40, 10, 10};
        c = {2, 3, 1};
    }

        // csak a 2. fázis kell
    if(eset == 4)
    {
            A = {
               {1, 0} ,
               {1, 1}
            };
            r = {1, 1};
            b = {2, 2};
            c = {1,1};
    }

        // nem korlatos problema
    if(eset == 5)
    {
            A = {
               {1, -1} ,
               {-2, 1}
            };
            r = {1, 1};
            b = {0, 0};
            c = {1,0};
    }

    Problema myProblem(A,r,b,c);
    myProblem.Simplex();

    return 0;
}
