stock bool:IsValidVehicleColour(colourid)
{
    return (colourid >= 0 && colourid < 256);
}

stock GetVehiclePaintjobCount(modelid)
{
    switch (modelid) {
        case 534, 535, 536, 558, 559, 560, 561, 562, 565, 567, 576:
            return 3;

        case 575:
            return 2;

        case 483:
            return 1;
    }

    return 0;
}

stock bool:IsValidPaintjob(modelid, paintjobid)
{
    new count = GetVehiclePaintjobCount(modelid);

    if (count == 0) {
        return false;
    }

    return (paintjobid >= 0 && paintjobid < count);
}
