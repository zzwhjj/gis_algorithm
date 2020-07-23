#include "ShapeReadWrite.h"

ShapeReadWrite::ShapeReadWrite()
{

}

ShapeReadWrite::~ShapeReadWrite()
{
    
}


 
int SHP_RW::Get_Point(XYZInfo& coordinate)
{
	OGRPoint *poPoint = (OGRPoint *)poGeometry;		//将几何结构转换成点类型
	coordinate.x = poPoint->getX();
	coordinate.y = poPoint->getY();
	coordinate.z = poPoint->getZ();
	return 0;
}
 
int SHP_RW::Get_LineString(vector<XYZInfo>& vecXYZ)
{
	OGRLineString* pLineGeo = (OGRLineString*)poGeometry;	//将几何结构转换成线类型
	int pointnums = pLineGeo->getNumPoints();
	XYZInfo tmpxyz;
	for (int i = 0; i < pointnums; i++)
	{
		tmpxyz.x = pLineGeo->getX(i);
		tmpxyz.y = pLineGeo->getY(i);
		tmpxyz.z = pLineGeo->getZ(i);
		vecXYZ.push_back(tmpxyz);
	}
	return 0;
}
 
//参数1是外环数据，是一个坐标名到坐标值vec的map，参数2是内环数据，比参数1多了一层编号映射
int SHP_RW::Get_Polygon(vector<XYZInfo>& OuterRing, vector<vector<XYZInfo>>& InteriorRing)
{
	OGRPolygon *poPolygon = (OGRPolygon *)poGeometry;	//将几何结构转换成多边形类型
	OGRLinearRing *pOGRLinearRing = poPolygon->getExteriorRing();
	//获取外环数据
	XYZInfo outring;
	for (int i = 0; i < pOGRLinearRing->getNumPoints(); i++)
	{
		outring.x = pOGRLinearRing->getX(i);
		outring.y = pOGRLinearRing->getY(i);
		outring.z = pOGRLinearRing->getZ(i);
		OuterRing.push_back(outring);
	}
 
	//获取内环数据（一个外环包含若干个内环）
	for (int i = 0; i < poPolygon->getNumInteriorRings(); i++)
	{
		vector<XYZInfo> Ringvec;
		pOGRLinearRing = poPolygon->getInteriorRing(i);
		for (int j = 0; j < pOGRLinearRing->getNumPoints(); j++)
		{
			XYZInfo intrring;
			intrring.x = pOGRLinearRing->getX(j);
			intrring.y = pOGRLinearRing->getY(j);
			intrring.z = pOGRLinearRing->getZ(j);
			Ringvec.push_back(intrring);
		}
		InteriorRing.push_back(Ringvec);
	}
	return 0;
}
 
//点集合
int SHP_RW::Get_MultiPoint(vector<XYZInfo> Points)
{
	OGRMultiPoint *poMultiPoint = (OGRMultiPoint *)poGeometry;		//将几何结构转换成点集合类型
	for (int i = 0; i < poMultiPoint->getNumGeometries(); i++)
	{
		OGRGeometry * geometry = poMultiPoint->getGeometryRef(i);	//根据下标获取点集合中的几何（点）结构
		OGRPoint *poPoint = (OGRPoint *)geometry;		//转换成点类型
		XYZInfo point;
		point.x = poPoint->getX();
		point.y = poPoint->getY();
		point.z = poPoint->getZ();
		Points.push_back(point);
	}
	return 0;
}
 
//线集合
int SHP_RW::Get_MultiLineString(vector<vector<XYZInfo>> Lines)
{
	OGRMultiLineString* poMultiLine = (OGRMultiLineString *)poGeometry;		//将几何结构转换成线集合类型
	for (int i = 0; i < poMultiLine->getNumGeometries(); i++)
	{
		vector<XYZInfo> Line;
		OGRGeometry * currtGeometry = poMultiLine->getGeometryRef(i);		//根据下标获取对应独立几何（线）结构
		OGRLineString* poLine = (OGRLineString*)currtGeometry;		//转换成线类型
		for (int j = 0; j < poLine->getNumPoints(); j++)
		{
			XYZInfo point;
			point.x = poLine->getX(j);
			point.y = poLine->getY(j);
			point.z = poLine->getZ(j);
			Line.push_back(point);
		}
		Lines.push_back(Line);
	}
	return 0;
}
 
//多边形集合
int SHP_RW::Get_MultiPolygon(vector<vector<XYZInfo>>& OuterRingVec, vector<vector<vector<XYZInfo>>>& InteriorRingVec)
{
	OGRMultiPolygon *MultiPolygon = (OGRMultiPolygon *)poGeometry;		//将几何结构转换成多边形集合类型
	int NumMPolygon = MultiPolygon->getNumGeometries();
	for (int k = 0; k < NumMPolygon; k++)
	{
		OGRGeometry * FirstGeometry = MultiPolygon->getGeometryRef(k);		//根据下标获取对应独立几何（多边形）结构
		OGRPolygon*poPolygon = (OGRPolygon *)FirstGeometry;			//转换成多边形类型
		OGRLinearRing *pOGRLinearRing = poPolygon->getExteriorRing();
		vector<XYZInfo> OuterRing;
		vector<vector<XYZInfo>> InnerRing;
		for (int i = 0; i < pOGRLinearRing->getNumPoints(); i++)
		{
			XYZInfo tmpinfo;
			tmpinfo.x = pOGRLinearRing->getX(i);
			tmpinfo.y = pOGRLinearRing->getY(i);
			tmpinfo.z = pOGRLinearRing->getZ(i);
			OuterRing.push_back(tmpinfo);
		}
		OuterRingVec.push_back(OuterRing);
		//获取内环数据（一个外环包含若干个内环）
		for (int i = 0; i < poPolygon->getNumInteriorRings(); i++)
		{
			vector<XYZInfo> InteriorRing;
			
			pOGRLinearRing = poPolygon->getInteriorRing(i);
			for (int j = 0; j < pOGRLinearRing->getNumPoints(); j++)
			{
				XYZInfo tmpinfo;
				tmpinfo.x = pOGRLinearRing->getX(i);
				tmpinfo.y = pOGRLinearRing->getY(i);
				tmpinfo.z = pOGRLinearRing->getZ(i);
				InteriorRing.push_back(tmpinfo);
			}
			InnerRing.push_back(InteriorRing);
		}
		InteriorRingVec.push_back(InnerRing);
	}
 
	return 0;
}
 
int SHP_RW::Get_GeometryCollection()
{
	return 0;
}
 
 
/* *************** 文件写入操作 ********************* */
int SHP_RW::Set_Point(int x, int y, int z, map<string, string> fieldvalue)
{
	OGRFeature *poFeature;
	if (poLayer == NULL)
		return -1;
	poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
	// 根据提供的字段值map对相应字段赋值
	for (auto field : fieldvalue)
	{
		poFeature->SetField(field.first.c_str(), field.second.c_str());
	}
	OGRPoint point;
	point.setX(x);
	point.setY(y);
	point.setZ(z);
	poFeature->SetGeometry((OGRGeometry *)(&point));
	if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
	{
		printf("Failed to create feature in shapefile.\n");
		return -1;
	}
	OGRFeature::DestroyFeature(poFeature);
 
	return 0;
}
 
//线元素	结构体传参
int SHP_RW::Set_LineString(vector<XYZInfo> Line, map<string, string> fieldvalue)
{
	OGRFeature *poFeature;
	if (poLayer == NULL)
		return -1;
	poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
	// 根据提供的字段值map对相应字段赋值
	for (auto field : fieldvalue)
	{
		poFeature->SetField(field.first.c_str(), field.second.c_str());
	}
 
	OGRLineString pLine;
 
	for (int i = 0; i < Line.size(); i++)
		pLine.addPoint(Line[i].x, Line[i].y, Line[i].z);
 
	poFeature->SetGeometry((OGRGeometry *)(&pLine));
	if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
	{
		printf("Failed to create feature in shapefile.\n");
		return -1;
	}
 
 
	OGRFeature::DestroyFeature(poFeature);
 
	return 0;
}
 
//线元素	参数分别为线要素上的点的x、y、z坐标的vector (z坐标可以没有)
int SHP_RW::Set_LineString(vector<double> vecX, vector<double> vecY, vector<double> vecZ, map<string, string> fieldvalue)
{
	OGRFeature *poFeature;
	if (poLayer == NULL)
		return -1;
	poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
	// 根据提供的字段值map对相应字段赋值
	for (auto field: fieldvalue)
	{
		poFeature->SetField(field.first.c_str(), field.second.c_str());
	}
 
	OGRLineString pLine;
	if (vecX.size() == vecY.size())
	{
		if(vecY.size() == vecZ.size())
			for (int i = 0; i < vecX.size(); i++)
				pLine.addPoint(vecX[i], vecY[i], vecZ[i]);
		else if(vecZ.size() == 0)	//z坐标为空
			for (int i = 0; i < vecX.size(); i++)
				pLine.addPoint(vecX[i], vecY[i], 0);
 
		poFeature->SetGeometry((OGRGeometry *)(&pLine));
		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			return -1;
		}
	}
	else
		return -1;
 
	OGRFeature::DestroyFeature(poFeature);
 
	return 0;
}
 
 
// 多边形图层(可以无内环）
int SHP_RW::Set_Polygon(vector<XYZInfo> OuterRing,
						vector<vector<XYZInfo>> InteriorRingVec,
						map<string, string> fieldvalue)
{
	try
	{
		OGRFeature *poFeature;
		if (poLayer == NULL)
			return -1;
		poFeature = OGRFeature::CreateFeature(poLayer->GetLayerDefn());
 
		// 根据提供的字段值map对相应字段赋值
		for (auto field : fieldvalue)
		{
			poFeature->SetField(field.first.c_str(), field.second.c_str());
		}
 
		//polygon
		OGRPolygon polygon;
		// 外环
		OGRLinearRing ringOut;
		for (int i = 0; i < OuterRing.size(); i++)
		{
			ringOut.addPoint(OuterRing[i].x, OuterRing[i].y, OuterRing[i].z);
		}
		//结束点应和起始点相同，保证多边形闭合
		ringOut.closeRings();
		polygon.addRing(&ringOut);
 
		// 内环
		for (int i = 0; i < InteriorRingVec.size(); i++)
		{
			OGRLinearRing ringIn;
			for (int j = 0; j < InteriorRingVec[i].size(); j++)
			{
				ringIn.addPoint(InteriorRingVec[i][j].x, InteriorRingVec[i][j].y, InteriorRingVec[i][j].z);
			}
			ringIn.closeRings();
			polygon.addRing(&ringIn);
		}
 
		poFeature->SetGeometry((OGRGeometry *)(&polygon));
		if (poLayer->CreateFeature(poFeature) != OGRERR_NONE)
		{
			printf("Failed to create feature in shapefile.\n");
			return -1;
		}
 
		OGRFeature::DestroyFeature(poFeature);
 
		return 0;
	}
	catch (std::exception&e)
	{
		//LogInfo(FormatLogStr("E", "ImageShp-> " + string(e.what()) + "!"));
		return -1;
	}
}
 
 
int SHP_RW::Set_MultiPoint()
{
	return 0;
}
 
int SHP_RW::Set_MultiLineString()
{
	return 0;
}
 
int SHP_RW::Set_MultiPolygon()
{
	return 0;
}
 
int SHP_RW::Set_GeometryCollection()
{
	return 0;
}
 
//创建属性字段
int SHP_RW::SetFieldDefn(vector<string> fieldname, vector<OGRFieldType> fieldtype, vector<int> fieldwidth)
{
	for (int i = 0; i < fieldname.size(); i++)
	{
		OGRFieldDefn Field(fieldname[i].c_str(), fieldtype[i]);	//创建字段 字段+字段类型
		Field.SetWidth(fieldwidth[i]);		//设置字段宽度，实际操作需要根据不同字段设置不同长度
		poLayer->CreateField(&Field);
	}
	return 0;
}
/* *************** 文件写入操作 ********************* */
