/**********************************************************************

 *<

	FILE: spline.h



	DESCRIPTION:  Defines a Spline Object Class



	CREATED BY: Tom Hudson



	HISTORY: created 23 February 1995



 *>	Copyright (c) 1995, All Rights Reserved.

 **********************************************************************/



#ifndef __SPLSHAPE_H__ 



#define __SPLSHAPE_H__



#include "shape.h"

extern CoreExport Class_ID  splineShapeClassID; 



class SplineShape : public ShapeObject {			   

	private:

		Interval geomValid;

		Interval topoValid;

		Interval selectValid;

		DWORD validBits; // for the remaining constant channels

		void CopyValidity(SplineShape *fromOb, ChannelMask channels);



	protected:

		//  inherited virtual methods for Reference-management

		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message );



	public:

		BezierShape		shape;



		CoreExport SplineShape();

		CoreExport ~SplineShape();



 		//  inherited virtual methods:



		// From BaseObject

		CoreExport int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt);

		CoreExport void Snap(TimeValue t, INode* inode, SnapInfo *snap, IPoint2 *p, ViewExp *vpt);

		CoreExport int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);

		CoreExport CreateMouseCallBack* GetCreateMouseCallBack();

		CoreExport RefTargetHandle Clone(RemapDir& remap = NoRemap());

		

		// From Object			 

		CoreExport ObjectState Eval(TimeValue time);

		CoreExport Interval ObjectValidity(TimeValue t);



		// The validty interval of channels necessary to do a convert to type

		CoreExport Interval ConvertValidity(TimeValue t);



		// get and set the validity interval for the nth channel

	   	CoreExport Interval ChannelValidity(TimeValue t, int nchan);

		CoreExport void SetChannelValidity(int i, Interval v);

		CoreExport void InvalidateChannels(ChannelMask channels);



		// Deformable object procs	

		int IsDeformable() { return 1; }  

		CoreExport int NumPoints();

		CoreExport Point3 GetPoint(int i);

		CoreExport void SetPoint(int i, const Point3& p);

		

		CoreExport void PointsWereChanged();

		CoreExport void GetDeformBBox(TimeValue t, Box3& box, Matrix3 *tm=NULL,BOOL useSel=FALSE );

		CoreExport void Deform(Deformer *defProc, int useSel);



		CoreExport int CanConvertToType(Class_ID obtype);

		CoreExport Object* ConvertToType(TimeValue t, Class_ID obtype);

		CoreExport void FreeChannels(ChannelMask chan);

		CoreExport Object *MakeShallowCopy(ChannelMask channels);

		CoreExport void ShallowCopy(Object* fromOb, ChannelMask channels);

		CoreExport void NewAndCopyChannels(ChannelMask channels);



		CoreExport DWORD GetSubselState();



		// From ShapeObject

		CoreExport int IntersectRay(TimeValue t, Ray& r, float& at);

		CoreExport ObjectHandle CreateTriObjRep(TimeValue t);  // for rendering, also for deformation		

		CoreExport void GetWorldBoundBox(TimeValue t, INode *inode, ViewExp* vpt, Box3& box );

		CoreExport void GetLocalBoundBox(TimeValue t, INode *inode, ViewExp* vpt, Box3& box );

		CoreExport int NumberOfCurves();

		CoreExport BOOL CurveClosed(TimeValue t, int curve);

		CoreExport Point3 InterpCurve3D(TimeValue t, int curve, float param);

		CoreExport Point3 TangentCurve3D(TimeValue t, int curve, float param);

		CoreExport float LengthOfCurve(TimeValue t, int curve);

		CoreExport int NumberOfPieces(TimeValue t, int curve);

		CoreExport Point3 InterpPiece3D(TimeValue t, int curve, int piece, float param);

		CoreExport Point3 TangentPiece3D(TimeValue t, int curve, int piece, float param);

		BOOL CanMakeBezier() { return TRUE; }

		CoreExport void MakeBezier(TimeValue t, BezierShape &shape);

		CoreExport ShapeHierarchy &OrganizeCurves(TimeValue t, ShapeHierarchy *hier = NULL);

		CoreExport void MakePolyShape(TimeValue t, PolyShape &shape, int steps = PSHAPE_BUILTIN_STEPS, BOOL optimize = FALSE);

		CoreExport int MakeCap(TimeValue t, MeshCapInfo &capInfo, int capType);

		CoreExport int MakeCap(TimeValue t, PatchCapInfo &capInfo);



		BezierShape& GetShape() { return shape; }



		// Animatable methods



		void DeleteThis() { delete this; }

		void FreeCaches() { shape.InvalidateGeomCache(); }

		Class_ID ClassID() { return splineShapeClassID; }

		void GetClassName(TSTR& s) { s = TSTR(_T("SplineShape")); }

		void NotifyMe(Animatable *subAnim, int message) {}

		int IsKeyable() { return 0;}

		int Update(TimeValue t) { return 0; }

		BOOL BypassTreeView() { return TRUE; }

		// This is the name that will appear in the history browser.

		TCHAR *GetObjectName() { return _T("Bezier Spline"); }



		// IO

		CoreExport IOResult Save(ISave *isave);

		CoreExport IOResult Load(ILoad *iload);



	};				



CoreExport ClassDesc* GetSplineShapeDescriptor();



#endif // __SPLSHAPE_H__

