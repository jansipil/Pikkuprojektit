import java.util.*;

//luokka pitää kirjaa pelin tilanteesta
class Hirsipuu {
	
	private String valittu_sana;
	private int arvaukset;

	private List<Character> arvatut_kirjaimet = new ArrayList<Character>();
	private List<Character> oikeat_kirjaimet = new ArrayList<Character>();

	//konstruktori arpoo sanan listalta
	public Hirsipuu(List<String> sanat, int arvausten_maara){

		Random rand = new Random();
		arvaukset = arvausten_maara;
		valittu_sana = sanat.get(rand.nextInt(sanat.size()));

		for(int a=0;a<valittu_sana.length();a++){
			oikeat_kirjaimet.add(valittu_sana.charAt(a));
		}

	}

	//tarkistaa onko merkki oikein
	//jos oikein vähentää oikeat merkit listalta sen merkin
	public boolean arvaa(Character merkki){

		boolean oikein = false;

		Iterator<Character> it_oikeat = oikeat_kirjaimet.iterator();

		arvatut_kirjaimet.add(merkki);

		for(int i=0; i<valittu_sana.length();i++){
			if(merkki == valittu_sana.charAt(i)){
				//System.out.println("Merkki oikein");
				oikein = true;

				while(it_oikeat.hasNext()){
					Character kohde = it_oikeat.next();
					if(merkki == kohde){
						it_oikeat.remove();
					}
				}
				break;
			}
		}
		if(oikein == false){
			//System.out.println("Merkki vaarin");
			arvaukset = arvaukset-1;
		}
		return oikein;
	}

	public List<Character> arvaukset(){
		return arvatut_kirjaimet;
	}

	public int arvauksiaOnJaljella(){
		return arvaukset;
	}

	public String sana(){
		return valittu_sana;
	}

	//tarkistaa voitetaanko peli jos oikeat merkit listalla ei enää ole mitään
	public boolean onLoppu(){

		boolean loppu = false;

		if(oikeat_kirjaimet.isEmpty()){
			loppu = true;
		}

		return loppu;
	}


}