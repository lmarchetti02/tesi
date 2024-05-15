// PROF. MONACO

#include "save_compton.hh"

#include "G4ios.hh"
#include "constants.hh"

void SaveCompton::Clear()
{
    for (int i = 0; i < list_info.size(); i++)
        delete list_info[i];
    list_info.clear();
}

void SaveCompton::AddStep(G4int partID, G4int parentID, G4int partType, G4int volID, G4double ene, G4double eneloss, G4int evid, G4String processName)
{
    if (partType == 0 && !list_info.size()) {
        // modified to register only one compton scattering
        // photon, check if the parentID is 1
        if (ene > min_ene) {
            ComptonInfo *STP = new ComptonInfo;
            STP->particle_id = partID;
            STP->parent_id = parentID;
            STP->part_type = partType;
            STP->volume_id = volID;
            STP->volume_id_iter = -1;
            STP->energy = ene;
            STP->eneLoss = 0;
            STP->eneLoss_iter = 0;
            STP->iter_found = 0;
            STP->iter_found_iter = 0;
            STP->escaped_ene = 0;
            list_info.push_back(STP);
        }
    } else if (volID < 0) {
        for (int j = 0; j < list_info.size(); j++) {
            if (parentID == list_info[j]->particle_id && list_info[j]->part_type == 0) {
                list_info[j]->escaped_ene = ene;
            }
        }
    } else {
        // scattered electron
        for (int j = 0; j < list_info.size(); j++) {
            if (parentID == list_info[j]->particle_id && list_info[j]->part_type == 0) {
                if (volID == list_info[j]->volume_id) {
                    list_info[j]->eneLoss += eneloss;
                    list_info[j]->iter_found++;
                } else {
                    list_info[j]->volume_id_iter = volID;
                    list_info[j]->iter_found_iter++;
                    list_info[j]->eneLoss_iter += eneloss;
                }
            }
        }
    }
}

G4int SaveCompton::PrintList(int evid)
{
    for (int i = 0; i < list_info.size(); i++) {
        G4cout << "COMPTON EvID=" << evid << " ene=" << list_info[i]->energy << " phID=" << list_info[i]->particle_id << " volID=" << list_info[i]->volume_id << " parentID=" << list_info[i]->parent_id
               << " volID=" << list_info[i]->volume_id << " ene_loss=" << list_info[i]->eneLoss << " n.iter=" << list_info[i]->iter_found << G4endl;
        G4cout << "    volID_iter=" << list_info[i]->volume_id_iter << " ph_ene=" << list_info[i]->energy << " ene_loss_iter=" << list_info[i]->eneLoss_iter
               << " n.iter2=" << list_info[i]->iter_found_iter << G4endl;
    }
    return (list_info.size());
}

G4double SaveCompton::GetComptonEnergy()
{
    if (!list_info.size())
        return -1.;
    else
        return list_info[0]->energy;
}

G4int SaveCompton::GetComptonID()
{
    if (!list_info.size())
        return 0;
    else {
        G4int ID = list_info[0]->volume_id;
        if (ID >= 0) {
            G4int i_s = ID / N_SUBPIXEL;
            G4int j_s = ID - i_s * N_SUBPIXEL;
            G4int i_m = i_s / PIXEL_RATIO;
            G4int j_m = j_s / PIXEL_RATIO;
            G4int ID_m = i_m * N_PIXEL + j_m;
            return ID_m;
        } else
            return ID;
    }
}

G4double SaveCompton::GetComptonEloss()
{
    if (!list_info.size())
        return -1.;
    else
        return list_info[0]->eneLoss;
}

G4int SaveCompton::GetComptonIDiter()
{
    if (!list_info.size())
        return 0;
    else {
        G4int ID = list_info[0]->volume_id_iter;
        if (ID >= 0) {
            G4int i_s = ID / N_SUBPIXEL;
            G4int j_s = ID - i_s * N_SUBPIXEL;
            G4int i_m = i_s / PIXEL_RATIO;
            G4int j_m = j_s / PIXEL_RATIO;
            G4int ID_m = i_m * N_PIXEL + j_m;
            return ID_m;
        } else
            return ID;
    }
}

G4double SaveCompton::GetComptonElossiter()
{
    if (!list_info.size())
        return -1.;
    else
        return list_info[0]->eneLoss_iter;
}
