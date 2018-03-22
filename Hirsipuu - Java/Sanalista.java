import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.*;

//luokka käsittelee sanalistatiedostoa
class Sanalista {

	private List<String> sanat = new ArrayList<String>();

	public Sanalista(String tiedoston_nimi){
		String line = null;
		try {
			FileReader fileReader = new FileReader(tiedoston_nimi);
			BufferedReader bufferedReader = new BufferedReader(fileReader);

			while((line = bufferedReader.readLine()) != null) {
				sanat.add(line);
			}

			bufferedReader.close();
		}
		catch(IOException ioe) {
			System.out.println("Tiedostoa ei voitu lukea");
		}
	}

	public Sanalista(List<String> lista){
		sanat = lista;
	}

	public List<String> annaSanat(){
		return sanat;
	}

	//metodia ei tällä hetkellä käytetä mutta sillä voisi poistaa listalta muut paitsi annetun pituuden mittaiset sanat
	public Sanalista sanatJoidenPituusOn(int pituus){

		List<String> uusilista = new ArrayList();
		Iterator<String> it_pituus = sanat.iterator();

		while (it_pituus.hasNext()) {
    		String sana = it_pituus.next();
    		if (sana.length() == pituus) {
       		uusilista.add(sana);
    		}
		}
		
		Sanalista pituuslista = new Sanalista(uusilista);
		
		return pituuslista;
	}
	
	//metodia ei käytetä mutta sillä voisi tarkistaa missä listan sanoissa merkit ovat tietyillä kohdilla
	//hyödyllinen jos peliä lähtisi vielä viemään eteenpäin
	public Sanalista sanaJoissaMerkit(String mjono){

		
		List<String> uusilista = new ArrayList();
		Iterator<String> it_merkit = sanat.iterator();

		for(int i=0;i<mjono.length();i++){
			if (mjono.charAt(i) != '_'){
				char kirjain1 = mjono.charAt(i);

				while (it_merkit.hasNext()) {
					String sana = it_merkit.next();
    				char kirjain2 = sana.charAt(i);
    				if (kirjain1 == kirjain2) {
       				uusilista.add(sana);
    				}
				}
			}
		}

		Sanalista merkkilista = new Sanalista(uusilista);
		
		return merkkilista;

	}


}