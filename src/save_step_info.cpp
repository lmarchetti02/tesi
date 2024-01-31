// PROF. MONACO

#include "save_step_info.hh"

void SaveStepInfo::Clear()
{
    for (int i = 0; i < list_info.size(); i++)
        delete list_info[i];
    list_info.clear();
}

void SaveStepInfo::AddStep(G4int partID, G4int parentID, G4int partType, G4int volID, G4double ene, G4double eneloss)
{
    if (partType == 0)
    {
        // photon, check if the parentID is 1
        if (partID != 1 && parentID == 1 && ene > min_ene)
        {
            // check if it is already in the list
            G4int found = -1;
            for (int j = 0; j < list_info.size(); j++)
            {
                if (list_info[j]->particle_id == partID && list_info[j]->volume_id > 0)
                {
                    found = j;
                    break;
                }
            }
            // if not already in the list add this step to the list of photons
            if (found < 0)
            {
                StepInfo *STP = new StepInfo;
                STP->particle_id = partID;
                STP->volume_id = volID;
                STP->energy = ene;
                STP->eneLoss = 0;
                STP->iter_found = 0;
                list_info.push_back(STP);
            }
            else
                list_info[found]->volume_id = volID;
        }
    }
    else if (partType < 0)
    {
        StepInfo *STP = new StepInfo;
        STP->particle_id = partID;
        STP->volume_id = partType;
        STP->energy = ene;
        STP->eneLoss = 0;
        STP->iter_found = 0;
        list_info.push_back(STP);
    }
    else
    {
        // check if it's parentID is a secondary photon registered in the list
        for (int j = 0; j < list_info.size(); j++)
        {
            if (parentID == list_info[j]->particle_id && parentID != 1)
            {
                StepInfo *STP = list_info[j];
                if (STP->iter_found && volID != STP->volume_id)
                {
                    volID = STP->volume_id;
                }
                STP->volume_id = volID;
                STP->eneLoss += eneloss;
                STP->iter_found++;
            }
        }
    }
}

void SaveStepInfo::PrintList(int evid)
{
    for (int i = 0; i < list_info.size(); i++)
    {
        G4cout << "FLUORO EvID=" << evid << " phID=" << list_info[i]->particle_id << " volID=" << list_info[i]->volume_id << " ph_ene=" << list_info[i]->energy << " ene_loss=" << list_info[i]->eneLoss << " n.iter=" << list_info[i]->iter_found << G4endl;
    }
}

std::vector<G4int> SaveStepInfo::GetIDchannelsVector()
{
    std::vector<G4int> idVec;
    // merge missing !!!!!
    for (int i = 0; i < list_info.size(); i++)
        idVec.push_back(list_info[i]->volume_id);
    return idVec;
}

std::vector<G4double> SaveStepInfo::GetEneVector()
{
    std::vector<G4double> eneVec;
    for (int i = 0; i < list_info.size(); i++)
        eneVec.push_back(list_info[i]->energy);
    return eneVec;
}

std::vector<G4double> SaveStepInfo::GetEneLossVector()
{
    std::vector<G4double> elossVec;
    for (int i = 0; i < list_info.size(); i++)
        elossVec.push_back(list_info[i]->eneLoss);
    return elossVec;
}
