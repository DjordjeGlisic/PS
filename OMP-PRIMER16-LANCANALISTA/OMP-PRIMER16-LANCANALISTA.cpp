#include<stdio.h>
#include<omp.h>
class Cvor {
public:
	int podatak;
	Cvor* sledeci;

	Cvor(int podatak) {
		this->podatak = podatak;
		sledeci = nullptr;
	}
};
class LancanaLista {
private:
	Cvor* glava;

public:
	LancanaLista() {
		glava = nullptr;
	}

	// Dodavanje čvora na kraj liste
	void dodaj_cvor(int podatak) {
		Cvor* novi = new Cvor(podatak);
		if (glava == nullptr) {
			glava = novi;
		}
		else {
			Cvor* trenutni = glava;
			while (trenutni->sledeci != nullptr) {
				trenutni = trenutni->sledeci;
			}
			trenutni->sledeci = novi;
		}
	}
	Cvor* vratiGlavu() { return glava; }



	// Uništavanje svih čvorova (destruktor)
	~LancanaLista() {
		Cvor* trenutni = glava;
		while (trenutni != nullptr) {
			Cvor* za_brisanje = trenutni;
			trenutni = trenutni->sledeci;
			delete za_brisanje;
		}
	}
};
int main(int argc, char* argv[])
{
	LancanaLista* lista = new LancanaLista();
#pragma omp parallel default(shared) num_threads(200)
	{
#pragma omp for schedule(static,1)
	for(int i=0;i<omp_get_num_threads();i++)
		lista->dodaj_cvor(i);
#pragma omp barrier
#pragma omp single
	{
		Cvor* trenutni = lista->vratiGlavu();
		printf("Nit%d koje je sama u sigle direktivi stampa podatak galve lancane liste:%d\n", omp_get_thread_num(), trenutni->podatak);
		while (trenutni != nullptr)
		{
#pragma omp task firstprivate(trenutni)
			{
				printf("Nit%d kojoj je dodeljen trenutni task stampa element liste:%d\n", omp_get_thread_num(), trenutni->podatak);
			}
#pragma omp taskwait
			trenutni=trenutni->sledeci;

		}
	}

	}
	return 0;
}